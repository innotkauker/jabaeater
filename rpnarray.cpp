#include "rpnarray.h"

void RPNarray::set(Token what, int where){
    if(where == -1)
        where = data.size();
    data[where] = what;
}
