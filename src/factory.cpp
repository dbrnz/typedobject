#include <string>
#include <map>
#include <memory>

#include <iostream>


class AObject {
 public:
  AObject() { }
  virtual ~AObject() { }
  virtual std::string name() = 0 ;
  } ;

class Recording : public AObject {
 public:
  std::string name() { return "recording" ; }
  } ;

class Signal : public AObject {
 public:
  std::string name() { return "signal" ; }
  } ;



// Factory code based on http://stackoverflow.com/a/16047779

class ProducerBase
/*==============*/
{
 public:
  ProducerBase() { }
  virtual ~ProducerBase() { }
  virtual std::unique_ptr<AObject> create() = 0 ;
  } ;


template<class T>
class Producer : public ProducerBase
/*================================*/
{
 public:
  Producer() { }
  virtual ~Producer() { }
  virtual std::unique_ptr<AObject> create() { return std::unique_ptr<AObject>(new T) ; }
  } ;


class AObjectFactory
/*================*/
{
 public:

  AObjectFactory()
  /*------------*/
  {
       // If you want, you can do all your Register() calls in here, and even
       // make the Register() function private.
    }

  void make(const std::string &key, std::unique_ptr<ProducerBase> producer)
  /*-----------------------------------------------------------*/
  {
    m_producers[key] = std::move(producer) ;
    }

  std::unique_ptr<AObject> create(const std::string &key)
  /*---------------------------------------------------*/
  {
    ProducerMap::iterator obj = m_producers.find(key) ;
    if (obj == m_producers.end()) return nullptr ;
    else                          return obj->second->create() ;
    }

 private:
  typedef std::map<std::string, std::unique_ptr<ProducerBase>> ProducerMap ;
  ProducerMap m_producers ;
  } ;



int main(void)
/*==========*/
{
  AObjectFactory factory ;

  factory.make("recording", std::unique_ptr<ProducerBase>(new Producer<Recording>)) ;
  factory.make("signal",    std::unique_ptr<ProducerBase>(new Producer<Signal>)) ;

  std::unique_ptr<AObject> s = factory.create("signal") ;
  std::unique_ptr<AObject> r = factory.create("recording") ;

  std::cout << s->name() << " | " << r->name() << std::endl ;
  }
