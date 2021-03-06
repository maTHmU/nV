#include "common.h"
#include <nV/Polynomial.h>
#include <nV/Number.h>
#include <algorithm>

namespace nV {
var Times(Kernel& k, const Tuple& x) {
    if (x.size == 1)
        return new Integer(1L);
    if (x.size == 2)
        return x[1];
    std::vector<var> r;
    //Collect constant term,x is sorted ?
    uint pos = 1;
    if (isNumber(x[1])) {
        var c = x[1].clone();
        for (pos = 2; pos < x.size && isNumber(x[pos]); ++pos)
            c = Number::Times(c.object(), x[pos].object());
        if(c.isObject($.Rational))
            mpq_canonicalize(c.cast<Rational>().mpq);
        if (!cmpD(c.object(), 0.0))
            return c;
        if (cmpD(c.object(), 1.0))
            r.push_back(c);
    }
    //typedef std::unordered_map<var, std::vector<var> > UMap;
    typedef std::map<var, std::vector<var> > UMap;
    UMap map;
    for (; pos < x.size; ++pos) {
        var b = x[pos], e;
        if (b == $.Infinity) {
            if (r.size() > 0 && cmpD(r[0].object(), 0.0) < 0)
                return tuple($.Times, new Integer(-1L), $.Infinity);
            return $.Infinity;
        }
        if (b.isTuple($.Power)) {//x^2->b=x,e=2
            const Tuple& t = b.tuple();
            b = t[1];
            e = t[2];
        } else
            e = new Integer(1L);
        map[b].push_back(e);
    }
    UMap::const_iterator iter = map.begin();
    while (iter != map.end()) {
        var b = iter->first;
        const std::vector<var>& v = iter->second;
        var e = nV::list(v.size(), v.begin(), $.Plus);
        ++iter;
        std::sort(e.tuple().tuple + 1, e.tuple().tuple + e.tuple().size, lessPrimary);
        e = Plus(k, e.tuple());
        if (isNumber(e)) {
            double ed = toD(e.object());
            if (ed == 0.0)
                continue;
            if (ed != 1.0)
                b = tuple($.Power, b, e);
        } else
            b = tuple($.Power, b, e);
        r.push_back(b);
    }
    if (r.size() == 0)
        return new Integer(1L);
    if (r.size() == 1)
        return r[0];
    /*
    if (t[0].isObject() && cmpFrac(t[0].object(), -1.0) == 0) {
        uint i;
        for (i = 1; i < t.size(); ++i)
            if (t[i].isTuple($.Plus)) {
                t[i] = Expand(t[0], t[i]);
                break;
            }
        if (i < t.size()) {
            if (t.size() == 2) return t[1];
    		return nV::list(t.size() - 1, t.begin() + 1, $.Times);
    	}
    }
    */
    return nV::list(r.size(), r.begin(), $.Times);
}
var Times(Kernel& k, const var& x, const var& y) {
    if (x == 0 || y == 0)
        return null;
    if (x.isObject() && y.isObject())
        return Number::Times(x.object(), y.object());
    var r = tuple($.Times, x, y);
    r = k.flatten($.Times, r.tuple());
    std::sort(r.tuple().tuple + 1, r.tuple().tuple + r.tuple().size, lessPrimary);
    return Times(k, r.tuple());
}
}

using namespace nV;

CAPI void CVALUE(System, Times)(Kernel& k, var& r, Tuple& x) {
	if (!x[0].isSymbol())
		return;
	r = Times(k, x);
}
