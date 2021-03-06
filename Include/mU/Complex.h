#pragma once
#include "Kernel.h"

namespace mU {
struct complex_t : obj_t {
	var re;
	var im;
	complex_t(const var& x, const var& y) : re(x), im(y) 
	{
		assert (NumQ(x) && NumQ(y));
	}
	virtual var tag() { return SYSTEM_SYM(Complex);  }
	virtual var normal() { return Ex(SYSTEM_SYM(Complex), Vec(re, im)); }
	virtual void print(wostream &f) {
        const bool is_real = ZeroQ(im);
        const bool is_purely_imaginary = ZeroQ(re);
        
        if (is_real)
        {
            Print(re, f);
        }
        else
        {
            if (is_purely_imaginary)
            {
                if (!OneQ(im))
                {
                    Print(im, f);
                }
                f << _W('I');
            }
            else
            {
                f << _W('(');
                Print(re, f);
                f << _W('+');
                if (!OneQ(im))
                {
                    Print(im, f);
                }
                f << _W("I)");
            }
        }
	}
};
inline var& Re(Var x) {
	assert(ObjQ(x, TAG(Complex)));

	return CObj<complex_t>(x).re;
}
inline var& Im(Var x) {
	assert(ObjQ(x, TAG(Complex)));

	return CObj<complex_t>(x).im;
}
inline var Complex(Var x, Var y)
{
	assert (NumQ(x) && NumQ(y));

	return new complex_t(x, y);
}
}