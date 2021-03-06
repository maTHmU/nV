#include <nV/Kernel.h>

namespace nV {
bool Kernel::set(sym x, const Key& y, const var& z) {
    if (y.kind() == Key::String) {
        if (x->name())
            return assign(x->symbol(y.toS()), z);
        else
            return x->set(y.toS(), z);
    }
    return false;
}
bool Kernel::set(Tuple& x, const Key& y, const var& z) {
    if (y.kind() == Key::Integer) {
        uint i = y.toUI();
        if (i < x.size) {
            x[i] = z;
            return true;
        }
    } else
        return set(x[0], y, z);
    return false;
}
bool Kernel::set(const var& x, const Key& y, const var& z) {
    switch (x.primary()) {
    case Primary::Symbol:
        return set(x.symbol(), y, z);
    case Primary::Tuple:
        return set(x.tuple(), y, z);
    }
    return false;
}
bool Kernel::set(const var& x, const Tuple& y, const var& z) {
    if (y[0] != $.List) {
        if (y[0].isKey()) {
            Tuple* t = y.clone();
            t->tuple[0] = get(x, y[0].key());
            var r = rewrite(t);
            return assign(r.tuple(), z);
        }
        return false;
    }
    bool flag = false;
    var r = x;
    for (uint i = 1; i < y.size && r != $.Fail; ++i) {
        var c = eval(y[i]);
        Key* k;
        if (!y[i].isObject())
            return false;
        if (y[i].object().type == $.Integer)
            k = key(c.cast<Integer>().toUI());
        else if (y[i].object().type == $.String)
            k = key(wstr(c.cast<String>().str.c_str()));
        else
            return false;
        if (i == y.size - 1)
            flag = set(r, *k, z);
        else
            r = slot(r, *k);
    }
    return flag;
}
}