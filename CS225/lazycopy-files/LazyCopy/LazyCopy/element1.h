#ifndef ELEMENT1_H
#define ELEMENT1_H
#include <iostream>
#include "abstract-element.h"
/*
 * A concrete class implementing AbstractElement interface
 * Trivial.
 */
namespace CS225 {

  class Element1 : public AbstractElement {
    public:
      Element1(int _val);
	  Element1(const Element1& element1);
      virtual int Get() const;
      virtual void Set(int new_val);
      virtual void Print() const;
  	virtual Element1* Clone() final;
    private:
      int val;
  };
}
#endif
