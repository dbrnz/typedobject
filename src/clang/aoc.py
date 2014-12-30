import os, sys

import clang.cindex
from clang.cindex import TypeKind, CursorKind



def generate_metaclass(aobject, metaclass):
#------------------------------------------
  return('''const rdf::URI& %s::metaclass(void) const
{
  static rdf::URI m_metaclass = %s ;
  return m_metaclass ;
  }\n''' % (aobject, metaclass))


class AssignFromRDF(object):
#---------------------------

  def __init__(self, aobject, base):
  #---------------------------------
    self._code = [
      'void %s::assign_from_rdf(const rdf::Node &property, const rdf::Node &value)' % aobject,
      '{']
    if base: self._code.append('  %s::assign_from_rdf(property, value) ;' % base)
    self._else = ''

  def add_property(self, name, kind, property):
  #--------------------------------------------
    self._code.append('  %sif (property == %s) m_%s = ' % (self._else, property, name)
                     + ('value.to_string()'      if kind == 'STRING'
                   else 'utils::make_uri(value)' if kind == 'URI'
                   else 'utils::isoformat_to_datetime(value)' if kind == 'DATETIME'
                   else 'value')
                     + ' ;')
    self._else = 'else '

  def __str__(self):
  #-----------------
    return '\n'.join(self._code) + '\n  }\n'



class Generator(object):
#=======================

  def __init__(self, file):
  #------------------------
    self._file = file
    self._namespaces = [ ]
    self._classes = [ ]
    self._class = None
    self._base = None
    self._properties = { }

  def save(self):
  #--------------
    if len(self._classes) == 0: return
    (path, f) = os.path.split(os.path.splitext(self._file)[0])
    code = ['#include "%s.h"' % f]
#    code.extend(['',
#                 'using namespace rdf ;',
#                 'using namespace bsml ;',
#                 ''])
    for c in self._classes:
      code.append('')
      mc = c[2].get('METACLASS')
      if mc and mc[0]:
        code.append(generate_metaclass(c[0], mc[0]))
      a = AssignFromRDF(c[0], c[1])
      for p, v in c[2].iteritems():
        if p != 'METACLASS': a.add_property(p, *v)
      code.append(str(a))
    fn = os.path.join(path, 'aoc_%s.cpp' % f)
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
      fullname = (('::'.join(self._namespaces) + '::') if self._namespaces else "") + self._class
      self._classes.append((fullname, self._base, self._properties))
      self._class = None
      self._base = None
      self._properties = { }

  def add_property(self, name, *params):
  #-------------------------------------
    if self._class is not None:
      self._properties[name] = params


class Parser(object):
#====================

  def __init__(self, source):
  #--------------------------
    index = clang.cindex.Index.create()
    self._tu = index.parse(source, ['-x', 'c++', '-DAOC_COMPILE', '-Xclang', '-fsyntax-only'])
    self._output = None
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
    assert(len(params) == int(count))
    return params

  def parse_property(self, name, cursor):
  #--------------------------------------
    for c in cursor.get_children():
      if c.kind == CursorKind.CALL_EXPR and c.displayname.startswith('_PARAMETERS_'):
        self._output.add_property(name, *self.parse_parameters(c, c.displayname[12:]))

  def parse(self, cursor):
  #-----------------------
    kind = cursor.kind
    name = cursor.displayname
    if (self._file is not None
     and cursor.location.file is not None
     and self._file != cursor.location.file.name): return

    if kind == CursorKind.TRANSLATION_UNIT:
      self._file = name
      self._output = Generator(name)
      self.parse_children(cursor)
      self._output.save()

    elif kind == CursorKind.NAMESPACE:
      self._output.add_namespace(name)
      self.parse_children(cursor)
      self._output.pop_namespace()

    elif kind == CursorKind.CLASS_DECL:
      self._class = name
      self.parse_children(cursor)
      self._output.end_class()
      self._class = None
      self._base = None
    
    elif kind == CursorKind.CXX_BASE_SPECIFIER and self._class:
      self._base = name

    elif cursor.kind == CursorKind.VAR_DECL and name == '_AOBJECT_DEFINITION':
      self._output.start_class(self._class, self._base)

    elif cursor.kind == CursorKind.VAR_DECL and name.startswith('_PROPERTY_'):
      self.parse_property(name[10:], cursor)


if __name__ == '__main__':
#=========================

  for f in sys.argv[1:]:
    p = Parser(f)
    p.generate()
