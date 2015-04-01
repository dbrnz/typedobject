import os, sys

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
  def _assignvalue(code, property, cleanup, assignment):
  #-----------------------------------------------------
    if cleanup == '':
      code.append('    %sif (property == %s) %s'
                % ('else ' if len(code) else '', property, assignment))
    else:
      code.append('    %sif (property == %s) {'
                % ('else ' if len(code) else '', property))
      code.append('      %s' % cleanup)        
      code.append('      %s' % assignment)
      code.append('      }')

  def add_property(self, name, kind, property, *options):
  #------------------------------------------------------
##    print (name, kind, property, options)  ######################
    if property == 'NONE': return
    name = 'm_%s' % name
    if 'OBJ' not in options or options[0] in ['SET', 'RSET']:
      cleanup = ''
    else:
      cleanup = 'if (%s != nullptr) delete %s ;' % (name, name)
    value = (('TypedObject::create<%s>(%s::subtypes(), value, graph)' % (kind, kind)) if 'OBJ' in options
         else 'value.to_string()'      if kind == 'std::string'
         else 'value.to_int()'         if kind == 'rdf::Integer'
         else 'value.to_float()'       if kind == 'rdf::Decimal'
         else 'rdf::URI(value)'        if kind == 'rdf::URI'
         else 'utils::Datetime(value)' if kind == 'utils::Datetime'
         else 'utils::Duration(value)' if kind == 'utils::Duration'
         else 'value')
    if options[0] in ['SET', 'RSET']:
      if 'OBJ' in options:
        assign = '\n'.join(['{',
                         '      %s *obj = %s ;' % (kind, value),
                         '      if (obj != nullptr) %s.insert(obj) ;' % name,
                         '      }'])
      else:
        assign = '%s.insert(%s) ;' % (name, value)
    else:
      assign = '%s = %s ;' % (name, value)
    if options[0] in ['REVERSE', 'RSET']:
      self._assignvalue(self._setreverse, property, cleanup, assign)
    else:
      self._assignvalue(self._setvalues, property, cleanup, assign)

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
      'void %s::save_as_rdf(const rdf::Graph &graph)' % cls,
      '{']
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
      if kind in ['std::string', 'rdf::Decimal', 'rdf::Integer']:
        valid = 'rdf::Literal::not_empty(%(name)s)'
      else:
        valid = '%(name)s.is_valid()'

      value = (     'rdf::Literal(%(name)s)' if kind == 'std::string'
               else 'rdf::Literal(%(name)s)' if kind == 'rdf::Integer'
               else 'rdf::Literal(%(name)s)' if kind == 'rdf::Decimal'
               else '%(name)s.to_literal()'  if kind == 'utils::Datetime'
               else '%(name)s.to_literal()'  if kind == 'utils::Duration'
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

  def __init__(self, cls, base):
  #-----------------------------
#    print "B:", base, " C:", constructor(base)
    ctr = cls + '::' + cls
    self._ctr = [
'''%(ctr)s(const std::string &uri, const rdf::Graph &graph)
: %(ctr)s(uri)
{
  this->add_metadata(graph) ;
  }

%(ctr)s(const std::string &uri)\n''' % {'ctr': ctr}]
    if base:
      self._ctr.append(': %s(uri)' % base)
      self._comma = ',\n  '
    else:
      self._comma = ': '
    self._preset = ['', '{']
    self._dtr = ['', '  }', '', '%(cls)s::~%(cls)s()' % {'cls': cls}, '{']

  def initialise(self, name, kind, prop, *options):
  #-----------------------------------------------
##    print name, kind, options
    member = 'm_%s' % name
    self._ctr.append('%s%s(' % (self._comma, member)
                  + ('nullptr' if ('OBJ' in options and options[0] not in ['SET', 'RSET'])
                else 'rdf::Literal::Constants::EMPTY_STRING'  if kind == 'std::string'
                else 'rdf::Literal::Constants::EMPTY_INTEGER' if kind == 'rdf::Integer'
                else 'rdf::Literal::Constants::EMPTY_DECIMAL' if kind == 'rdf::Decimal'
#                else '0'   if kind == 'utils::Datetime'
#                else '0.0' if kind == 'utils::Duration'
                else '')
                   + ')')
    self._comma = ',\n  '
    if 'OBJ' in options:
      if options[0] not in ['SET', 'RSET']:
        self._dtr.append('  if (%s != nullptr) delete %s ;' % (member, member))
      else:
        self._dtr.append('  for (auto const &e: %s) {' % member)
        self._dtr.append('    if (e != nullptr) delete e ;')
        self._dtr.append('    }')

  def preset(self, name, value):
  #-----------------------------
    self._preset.append('  set_%s(%s) ;' % (name, value))


## Only if there are restrictions/presets...
#  bool CLS::satisfies_restrictions(const rdf::Graph &graph)
#  {
##
## We want to go from name of attribute (format) to its property (DCT::format)
## Have this at (super-)class declaration in PROPERTY_type(attribute, property) statement.
##
## Each class has a (static) map, initially the same as the parent's
##
#    return (graph.contains(uri(), m_filterprop1, m_filtervalue1)
#         && graph.contains(uri(), m_filterprop2, m_filtervalue2)
#         && ...) ;
#    }


  def __str__(self):
  #-----------------
    return ''.join(self._ctr) + '\n'.join(self._preset) + '\n'.join(self._dtr) + '\n  }\n\n'


class Generator(object):
#=======================

  def __init__(self, file):
  #------------------------
    self._file = file
    self._namespaces = [ ]
    self._classes = [ ]
    self._class = None
    self._base = None
    self._properties = ({ }, { }, { })
    self._usednames = [ ]

  def save(self, hdr, fn):
  #-----------------------
    if len(self._classes) == 0: return
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
      c = Constructor(cls[1], cls[2])
      a = AssignFromRDF(cls[1], cls[2])
      s = SaveToRDF(cls[1], cls[2])
      for p, v in cls[3][0].iteritems():
        if p != 'TYPE':
          c.initialise(p, *v)
          a.add_property(p, *v)
          s.save_property(p, *v)
      for p, v in cls[3][1].iteritems():
        a.add_property(p, *v)
        s.save_property(p, *v)
      for p, v in cls[3][2].iteritems():  ###
        c.preset(p, v[0])
      code.append(str(c))
      code.append(str(a))
      code.append(str(s))
      for n in namespaces:
        code.append('} ;')
        code.append('')
    output = open(fn, 'wb')
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
      self._properties = ({ }, { }, { })

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

  def parse_parameters(self, cursor, count):
  #-----------------------------------------
    params = [ ]
    for c in cursor.get_children():
      if c.kind == CursorKind.UNEXPOSED_EXPR and c.type.kind == TypeKind.POINTER:
        p = c.type.get_pointee()
        if p.kind == TypeKind.CHAR_S and p.is_const_qualified():
          s = list(c.get_children())[0]
          if s.kind == CursorKind.STRING_LITERAL and s.type.kind == TypeKind.CONSTANTARRAY:
            params.append(s.displayname[1:-1])
    assert(len(params) >= int(count))
    return params

  def parse_property(self, name, cursor):
  #--------------------------------------
    for c in cursor.get_children():
      if c.kind == CursorKind.CALL_EXPR and c.displayname.startswith('_PARAMETERS_'):
        self._generator.add_property(name, *self.parse_parameters(c, c.displayname[12:]))

  def parse_assignment(self, name, cursor):
  #----------------------------------------
    for c in cursor.get_children():
      if c.kind == CursorKind.CALL_EXPR and c.displayname.startswith('_PARAMETERS_'):
        self._generator.add_assignment(name, *self.parse_parameters(c, c.displayname[12:]))

  def parse_restriction(self, name, cursor):
  #-----------------------------------------
    for c in cursor.get_children():
      if c.kind == CursorKind.CALL_EXPR and c.displayname.startswith('_PARAMETERS_'):
        self._generator.add_restriction(name, *self.parse_parameters(c, c.displayname[12:]))

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

    elif cursor.kind == CursorKind.VAR_DECL and name == '_OBJECT_DEFINITION':
      self._generator.start_class(self._class, self._base)

    elif cursor.kind == CursorKind.VAR_DECL and name.startswith('_PROPERTY_'):
      self.parse_property(name[10:], cursor)

    elif cursor.kind == CursorKind.VAR_DECL and name.startswith('_ASSIGN_'):
      self.parse_assignment(name[8:], cursor)

    elif cursor.kind == CursorKind.VAR_DECL and name.startswith('_RESTRICT_'):
      self.parse_restriction(name[10:], cursor)

    elif cursor.kind == CursorKind.USING_DIRECTIVE:
      self.parse_using(cursor)


if __name__ == '__main__':
#=========================

  if len(sys.argv) < 4:
    sys.exit("Usage: %s [OPTIONS] SOURCE_DIR HEADER_FILE OUTPUT_PATH")
  p = Parser(sys.argv[-3], sys.argv[-2], sys.argv[-1], sys.argv[1:-3])
  p.generate()
