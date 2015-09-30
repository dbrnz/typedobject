#*****************************************************************************
#*                                                                           *
#*  Typed RDF Objects for C++                                                *
#*                                                                           *
#*  Copyright (c) 2014-2015  David Brooks                                    *
#*                                                                           *
#*  Licensed under the Apache License, Version 2.0 (the "License");          *
#*  you may not use this file except in compliance with the License.         *
#*  You may obtain a copy of the License at                                  *
#*                                                                           *
#*      http://www.apache.org/licenses/LICENSE-2.0                           *
#*                                                                           *
#*  Unless required by applicable law or agreed to in writing, software      *
#*  distributed under the License is distributed on an "AS IS" BASIS,        *
#*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
#*  See the License for the specific language governing permissions and      *
#*  limitations under the License.                                           *
#*                                                                           *
#*****************************************************************************

import os, sys
import datetime
from collections import OrderedDict

import clang.cindex
from clang.cindex import TypeKind, CursorKind



def generate_types(T, cls, base):
#--------------------------------
  return('''const rdf::URI& %(cls)s::type(void) const
{
  static rdf::URI m_type = %(T)s ;
  return m_type ;
  }
  
std::set<rdf::URI> &%(cls)s::subtypes(void)
{
  static std::set<rdf::URI> m_subtypes ;
  return m_subtypes ;
  }
  

int %(cls)s::add_subtype(const rdf::URI &T)
{
  %(cls)s::subtypes().insert(T) ;
  return 0 ;
  }

REGISTER_TYPE(%(T)s, %(cls)s)
REGISTER_SUBTYPE(%(T)s, %(cls)s, %(base)s)
''' % {'T': T, 'cls': cls, 'base': base})


class AssignFromRDF(object):
#---------------------------

  def __init__(self, cls, base):
  #-----------------------------
    self._header = [
      'void %s::assign_from_rdf(const rdf::Graph &graph, const rdf::Node &property,' % cls,
      '                         const rdf::Node &value,  const bool reverse)',
      '{']
    if base and base != 'TypedObject::TypedObject':
      self._header.append('  %s::assign_from_rdf(graph, property, value, reverse) ;' % base)
    self._setvalues = []
    self._setreverse = []

  @staticmethod
  def _assignvalue(code, property, assignment, elseif):
  #----------------------------------------------------
    code.append('    %sif (property == %s) %s'
              % ('else ' if elseif and len(code) else '', property, assignment))

  def add_property(self, name, kind, property, *options):
  #------------------------------------------------------
##    print (name, kind, property, options)  ######################
    if property == 'NONE': return
    name = 'm_%s' % name
    ## Could optimise reverse case by looking up type(s) of found object once only
    ## Currently done for each value in `std::shared_ptr<T> TypedObject::create()`
    value = (('TypedObject::create<%s>(%s::subtypes(), value, graph)' % (kind, kind)) if 'OBJ' in options
         else 'value.to_string()'    if kind == 'std::string'
         else 'value.to_int()'       if kind == 'xsd::Integer'
         else 'value.to_float()'     if kind == 'xsd::Decimal'
         else 'rdf::URI(value)'      if kind == 'rdf::URI'
         else 'xsd::Datetime(value)' if kind == 'xsd::Datetime'
         else 'xsd::Duration(value)' if kind == 'xsd::Duration'
         else 'value')
    if options[0] in ['SET', 'RSET']:
      if 'OBJ' in options:
        assign = '\n'.join(['{',
                         '      std::shared_ptr<%s> obj = %s ;' % (kind, value),
                         '      if (obj) %s.insert(obj) ;' % name,
                         '      }'])
      else:
        assign = '%s.insert(%s) ;' % (name, value)
    else:
      assign = '%s = %s ;' % (name, value)
    if options[0] in ['REVERSE', 'RSET']:
      self._assignvalue(self._setreverse, property, assign, False)
    else:
      self._assignvalue(self._setvalues, property, assign, True)

  def __str__(self):
  #-----------------
    if self._setvalues:
      return '\n'.join(self._header
                      + ['  if (!reverse) {'] + self._setvalues
                      + ((['    }', '  else {'] + self._setreverse) if self._setreverse else [])
                      + ['    }', '  }', ''])
    elif self._setreverse:
      return '\n'.join(self._header
                      + ['  if (reverse) {'] + self._setreverse
                      + ['    }', '  }', ''])
    else:
      return '\n'.join(self._header + ['  }', ''])


class SaveToRDF(object):
#-----------------------

  def __init__(self, cls, base):
  #-----------------------------
    self._header = [
      'void %s::save_as_rdf(rdf::Graph & graph)' % cls,
      '{'
      '  graph.add_prefixes(m_prefixes) ;'
      '  graph.add_prefixes(s_prefixes) ;'
      ]
    self._footer = ['  }', '']
    if base and base != 'TypedObject::TypedObject':
      self._footer.insert(0, '  %s::save_as_rdf(graph) ;' % base)
    self._getvalues = []

  def save_property(self, name, kind, property, *options):
  #-------------------------------------------------------
    if property == 'NONE': return

    if options[0] in ['REVERSE', 'RSET']:
      save = '  for (auto const &value : %(name)s) value->save_metadata(graph) ;'
    elif 'OBJ' not in options or options[0] == 'SET':
      # Only save non-empty values...
      if kind in ['std::string', 'xsd::Decimal', 'xsd::Integer']:
        valid = 'rdf::Literal::not_empty(%(name)s)'
      else:
        valid = '%(name)s.is_valid()'
      value = (     'rdf::Literal(%(name)s)' if kind == 'std::string'
               else 'rdf::Literal(%(name)s)' if kind == 'xsd::Integer'
               else 'rdf::Literal(%(name)s)' if kind == 'xsd::Decimal'
               else '%(name)s.to_literal()'  if kind == 'xsd::Datetime'
               else '%(name)s.to_literal()'  if kind == 'xsd::Duration'
               else '%(name)s')
      if options[0] == 'SET':
        save = ('  for (auto const &value : %(name)s)'
                + ' if (%s)' % (valid % {'name': 'value'})
                + ' graph.insert(uri(), DCT::source, %s) ;' % (value % {'name': 'value'})
                )
      else:
        save = ('  if (%s)' % valid
                + ' graph.insert(uri(), %(prop)s, ' + value + ') ;'
                )
    elif 'OBJ' in options:
      save = """  if (%(name)s != nullptr) {
    graph.insert(uri(), %(prop)s, %(name)s->uri()) ;
    %(name)s->save_metadata(graph) ;
    }"""
    self._getvalues.append(save % { 'prop': property, 'name': 'm_' + name})

  def __str__(self):
  #-----------------
    return '\n'.join(self._header + self._getvalues + self._footer)


class Constructor(object):
#-------------------------

  def __init__(self, cls, base, init_code, prefixes):
  #--------------------------------------------------
#    print "B:", base, " C:", constructor(base)
    self._class = cls
    ctr = cls + '::' + cls
    self._hdr = '''%(ctr)s(const rdf::URI &uri, const rdf::Graph &graph)
: %(ctr)s(uri)
{
  if (!this->add_metadata(graph)) *this = %(cls)s() ;
  }

%(ctr)s(const rdf::URI &uri)\n''' % {'ctr': ctr, 'cls': cls}
    b = base.split('::')
    if len(b) > 1 and b[-2] == b[-1]: del b[-1]
    self._ctr = [': %s(uri)' % '::'.join(b),
                 '  m_prefixes(std::set<rdf::Namespace>())',
                ]
    self._base = '::'.join(b)
    self._props = [ ]
    self._preset = ['{']
    self._restrict = [ ]
    self._init_code = init_code
    self._prefixes = prefixes

  def initialise(self, name, kind, prop, *options):
  #-----------------------------------------------
##    print name, kind, options
    member = 'm_%s' % name
    self._ctr.append('  %s(' % member
                  + ('nullptr' if ('OBJ' in options and options[0] not in ['SET', 'RSET'])
                else 'rdf::Literal::Constants::EMPTY_STRING'  if kind == 'std::string'
                else 'rdf::Literal::Constants::EMPTY_INTEGER' if kind == 'xsd::Integer'
                else 'rdf::Literal::Constants::EMPTY_DECIMAL' if kind == 'xsd::Decimal'
                else '')
                   + ')')

  def add_property(self, name, kind, property, *options):
  #------------------------------------------------------
    if property != 'NONE':
      self._props.append('    {"%s", %s},' % (name, property))

  def preset(self, name, value, kind):
  #-----------------------------------
    self._preset.append('  set_%s(%s) ;' % (name, value))
    self._restrict.append('graph.contains(uri(), get_property("%s"), %s)'
      % (name, ('rdf::Literal(%(cast)s(%(value)s))'
                   if kind in ['std::string', 'xsd::Integer', 'xsd::Decimal']
           else '%(value)s.to_literal()'
                   if kind in ['xsd::Datetime', 'xsd::Duration']
           else '%(value)s') % {'value': value, 'cast': kind}))

  def __str__(self):
  #-----------------
    code = [self._hdr,
            ',\n'.join(self._ctr),
            '\n'.join(self._preset),
            '\n'.join(self._init_code) + '  }\n' + '\n',
           ]
    code.append('const std::set<rdf::Namespace> %s::s_prefixes {%s} ;\n'
                                         % (self._class, ', '.join(self._prefixes)))
    code.append('std::map<std::string, rdf::Node> %s::s_properties {' % self._class)
    code.extend(self._props)
    code.append('  } ;')
    code.append('''
void %(cls)s::add_prefix(const rdf::Namespace &prefix)
{
  m_prefixes.insert(prefix) ;
  }

rdf::Node %(cls)s::get_property(const std::string & name)
{
  auto entry = s_properties.find(name) ;
  if (entry != s_properties.end()) return entry->second ;
  else                             return %(base)s::get_property(name) ;
  }

bool %(cls)s::satisfies_restrictions(const rdf::Graph &graph)
{
  return %(base)s::satisfies_restrictions(graph)%(cond)s ;
  }
''' % {'cls': self._class, 'base': self._base,
       'cond': ('\n  && ' + '\n && '.join(self._restrict)) if self._restrict else ''
      })
    return '\n'.join(code)


class Generator(object):
#=======================

  def __init__(self, file):
  #------------------------
    self._file = file
    self._namespaces = [ ]
    self._classes = [ ]
    self._class = None
    self._base = None
    #  (properties, assignments, restrictions, init, prefixes)
    self._properties = (OrderedDict(), OrderedDict(), OrderedDict(), [ ], [ ])
    self._usednames = [ ]

  def save(self, hdr, fn):
  #-----------------------
    if len(self._classes) == 0: return
    output = open(fn, 'wb')
    output.write('// Generated at: %s\n' % datetime.datetime.now())
    code = ['#include "%s"' % hdr]
    if self._usednames:
      code.append('')
      for n in self._usednames:
        code.append('using namespace %s ;' % '::'.join(n))
      code.append('')
    for cls in self._classes:
      code.append('')
      namespaces = cls[0].split('::')
      for n in namespaces:
        code.append('namespace %s {' % n)
        code.append('')
      mcls = cls[3][0].get('TYPE')
      if mcls and mcls[0]:
        code.append(generate_types(mcls[0], cls[1], cls[2]))
      c = Constructor(cls[1], cls[2], cls[3][3], cls[3][4])  # class, base, init, prefixes
      a = AssignFromRDF(cls[1], cls[2])
      s = SaveToRDF(cls[1], cls[2])
      for p, v in cls[3][0].iteritems():  # properties
        if p != 'TYPE':
          c.initialise(p, *v)
          c.add_property(p, *v)
          a.add_property(p, *v)
          s.save_property(p, *v)
      for p, v in cls[3][1].iteritems():  # assignments
        c.add_property(p, *v)
        a.add_property(p, *v)
        s.save_property(p, *v)
      for p, v in cls[3][2].iteritems():  # restrictions
        c.preset(p, *v)
      code.append(str(c))
      code.append(str(a))
      code.append(str(s))
      for n in namespaces:
        code.append('} ;')
        code.append('')
    output.write('\n'.join(code))
    output.close()

  def add_namespace(self, name):
  #-----------------------------
    self._namespaces.append(name)

  def pop_namespace(self):
  #-----------------------
    del self._namespaces[-1]

  def start_class(self, name, base):
  #---------------------------------
    self._class = name
    self._base = base

  def end_class(self):
  #-------------------
    if self._class is not None:
      self._classes.append(('::'.join(self._namespaces), self._class, self._base, self._properties))
      self._class = None
      self._base = None
      #  (properties, assignments, restrictions, init, prefixes)
      self._properties = (OrderedDict(), OrderedDict(), OrderedDict(), [ ], [ ])

  def add_property(self, name, *params):
  #-------------------------------------
    if self._class is not None:
      self._properties[0][name] = params

  def add_assignment(self, name, *params):
  #-------------------------------------
    if self._class is not None:
      self._properties[1][name] = params

  def add_restriction(self, name, *params):
  #----------------------------------------
    if self._class is not None:
      self._properties[2][name] = params

  def add_initialisation(self, *params):
  #-------------------------------------
    if self._class is not None:
      for c in params:
        self._properties[3].append(c.replace('\\"', '"'))

  def add_prefixes(self, *params):
  #-------------------------------
    if self._class is not None:
      for c in params:
        self._properties[4].append(c)

  def use_namespace(self, name):
  #-----------------------------
    self._usednames.append([name])
    return self._usednames[-1]


class Parser(object):
#====================

  def __init__(self, path, header, output, options=[]):
  #----------------------------------------------------
    index = clang.cindex.Index.create()
    defopts = ['-x', 'c++', '-DTYPED_OBJECT_COMPILE', '-Xclang', '-fsyntax-only']
    self._header = header
    self._tu = index.parse(os.path.join(path, header), defopts + options)
    self._output = output
    self._generator = None
    self._class = None
    self._base = None
    self._file = None

  def generate(self):
  #------------------
    self.parse(self._tu.cursor)

  def parse_children(self, cursor):
  #--------------------------------
    for c in cursor.get_children():
      self.parse(c)

  def parse_type(self, cursor):
  #----------------------------
    for c in cursor.get_children():
      if c.kind == CursorKind.TYPE_REF and c.displayname.startswith('class '):
        return c.displayname[6:]

  def parse_parameters(self, cursor):
  #----------------------------------
    count = 0
    params = [ ]
    for c in cursor.get_children():
      if c.kind == CursorKind.UNEXPOSED_EXPR and c.type.kind == TypeKind.POINTER:
        p = c.type.get_pointee()
        if p.kind == TypeKind.CHAR_S and p.is_const_qualified():
          s = list(c.get_children())[0]
          if s.kind == CursorKind.STRING_LITERAL and s.type.kind == TypeKind.CONSTANTARRAY:
            if count == 0: count = int(s.displayname[1:-1])
            else:
              p = s.displayname[1:-1]  # strip ""
              if p.startswith('\\"'): p = p[2:-2]
              params.append(p)
    assert(len(params) >= int(count))
    assert(len(params) >= count)
    return params

  def parse_property(self, name, cursor):
  #--------------------------------------
    for c in cursor.get_children():
      if c.kind == CursorKind.CALL_EXPR and c.displayname == '_PARAMETERS_':
        self._generator.add_property(name, *self.parse_parameters(c))

  def parse_assignment(self, name, cursor):
  #----------------------------------------
    for c in cursor.get_children():
      if c.kind == CursorKind.CALL_EXPR and c.displayname == '_PARAMETERS_':
        self._generator.add_assignment(name, *self.parse_parameters(c))

  def parse_restriction(self, name, cursor):
  #-----------------------------------------
    for c in cursor.get_children():
      if c.kind == CursorKind.CALL_EXPR and c.displayname == '_PARAMETERS_':
        self._generator.add_restriction(name, *self.parse_parameters(c))

  def parse_initialisation(self, cursor):
  #--------------------------------------
    for c in cursor.get_children():
      if c.kind == CursorKind.CALL_EXPR and c.displayname == '_PARAMETERS_':
        self._generator.add_initialisation(*self.parse_parameters(c))

  def parse_prefixes(self, cursor):
  #--------------------------------
    for c in cursor.get_children():
      if c.kind == CursorKind.CALL_EXPR and c.displayname == '_PARAMETERS_':
        self._generator.add_prefixes(*self.parse_parameters(c))

  def parse_using(self, cursor):
  #-----------------------------
    ns = None
    for c in cursor.get_children():
      if c.kind == CursorKind.NAMESPACE_REF:
        if ns is None: ns = self._generator.use_namespace(c.displayname)
        else:          ns.append(c.displayname)

  def parse(self, cursor):
  #-----------------------
    kind = cursor.kind
    name = cursor.displayname
    if (self._file is not None
     and cursor.location.file is not None
     and self._file != cursor.location.file.name): return

    if kind == CursorKind.TRANSLATION_UNIT:
      self._file = name
      self._generator = Generator(name)
      self.parse_children(cursor)
      self._generator.save(self._header, self._output)

    elif kind == CursorKind.NAMESPACE:
      self._generator.add_namespace(name)
      self.parse_children(cursor)
      self._generator.pop_namespace()

    elif kind == CursorKind.CLASS_DECL:
      self._class = name
      self.parse_children(cursor)
      self._generator.end_class()
      self._class = None
      self._base = None
    
    elif kind == CursorKind.CXX_BASE_SPECIFIER and self._class:
      self._base = self.parse_type(cursor)

    elif kind == CursorKind.VAR_DECL and name == '_OBJECT_DEFINITION':
      self._generator.start_class(self._class, self._base)

    elif kind == CursorKind.VAR_DECL and name.startswith('_PROPERTY_'):
      self.parse_property(name[10:], cursor)

    elif kind == CursorKind.VAR_DECL and name.startswith('_ASSIGN_'):
      self.parse_assignment(name[8:], cursor)

    elif kind == CursorKind.VAR_DECL and name.startswith('_RESTRICT_'):
      self.parse_restriction(name[10:], cursor)

    elif kind == CursorKind.VAR_DECL and name == '_INITIALISE_':
      self.parse_initialisation(cursor)

    elif kind == CursorKind.VAR_DECL and name == '_PREFIXES_':
      self.parse_prefixes(cursor)

    elif kind == CursorKind.USING_DIRECTIVE:
      self.parse_using(cursor)


if __name__ == '__main__':
#=========================

  if len(sys.argv) < 4:
    sys.exit("Usage: %s [OPTIONS] SOURCE_DIR HEADER_FILE OUTPUT_PATH")
  p = Parser(sys.argv[-3], sys.argv[-2], sys.argv[-1], sys.argv[1:-3])
  p.generate()
