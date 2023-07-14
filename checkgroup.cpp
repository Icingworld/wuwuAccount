#include "checkgroup.h"

Checkgroup::Checkgroup()
{

}

Checkgroup::~Checkgroup()
{

}

void Checkgroup::add(QCheckBox * checkbox)
{
    list.append(checkbox);
}

void Checkgroup::select(const int index)
{
    for (int i=0;i<list.length();i++) {
        if (i != index)
        {
            list[i]->setChecked(false);
        } else {
            list[i]->setChecked(true);
        }
    }
    lastIndex = index;
}

/* whether none of the list is selected */
bool Checkgroup::isEmpty() const
{
    for (int i=0;i<list.length();i++) {
        if (list[i]->isChecked())
        {
            return false;
        }
    }
    return true;
}

/* return the index selected */
int Checkgroup::currentSelectIndex() const
{
    if (!isEmpty())
    {
        return lastIndex;
    }
    return -1;
}