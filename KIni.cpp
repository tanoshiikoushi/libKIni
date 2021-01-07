#include "KIni.h"
#include <iostream>

// toString Methods
std::string KIniComment::toString()
{
    return "//" + getValue() + "\n";
}

std::string KIniProperty::toString()
{
    return getPropertyName() + " = " + getPropertyValue() + "\n";
}

std::string KIniInstance::toString()
{
    std::string out = "";
    ListMember<KIniProperty>* index = getPropertyList();

    while (index != nullptr)
    {
        if (index->isComment())
        {
            out.append(index->getCurrentComment()->toString());
        }
        else
        {
            out.append(index->getCurrentContained()->toString());
        }
        index = index->getNextListMember();
    }
    out.append("\n");
    return out;
}

std::string KIniElement::toString()
{
    std::string out = "";
    ListMember<KIniInstance>* index = getInstanceList();

    out.append("name " + getElementName() + "\n\n");

    while (index != nullptr)
    {
        if (index->isComment())
        {
            out.append(index->getCurrentComment()->toString());
        }
        else
        {
            out.append(index->getCurrentContained()->toString());
        }
        index = index->getNextListMember();
    }
    return out;
}

std::string KIniRoot::toString()
{
    std::string out = "";
    ListMember<KIniElement>* index = getElementList();

    while (index != nullptr)
    {
        if (index->isComment())
        {
            out.append(index->getCurrentComment()->toString());
        }
        else
        {
            out.append(index->getCurrentContained()->toString());
        }
        index = index->getNextListMember();
    }
    return out;
}

// KIniInstance Methods
bool KIniInstance::insertPropertyAtIndex(unsigned long long index, KIniProperty* to_add)
{
    ListMember<KIniProperty>* curr_index = getPropertyList();

    if (curr_index == nullptr)
    {
        if (index == 0)
        {
            setPropertyList(new ListMember<KIniProperty>(0, false, to_add, nullptr, nullptr));
            return true;
        }
        else { return false; }
    }
    else
    {
        unsigned long long ind = index;
        while (ind > 1)
        {
            if (curr_index->getNextListMember()) { return false; }
            else
            {
                curr_index = curr_index->getNextListMember();
                ind--;
            }
        }

        ListMember<KIniProperty>* prior = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            prior->setNextListMember(new ListMember<KIniProperty>(index, false, to_add, nullptr, nullptr));
        }
        else
        {
            curr_index = curr_index->getNextListMember();
            prior->setNextListMember(new ListMember<KIniProperty>(index, false, to_add, nullptr, curr_index));

            curr_index->setListIndex(curr_index->getListIndex()+1);

            while (curr_index->getNextListMember() != nullptr)
            {
                curr_index = curr_index->getNextListMember();
                curr_index->setListIndex(curr_index->getListIndex()+1);
            }
        }
        return true;
    }
}

bool KIniInstance::insertCommentAtIndex(unsigned long long index, KIniComment* to_add)
{
    ListMember<KIniProperty>* curr_index = getPropertyList();

    if (curr_index == nullptr)
    {
        if (index == 0)
        {
            setPropertyList(new ListMember<KIniProperty>(0, true, nullptr, to_add, nullptr));
            return true;
        }
        else { return false; }
    }
    else
    {
        unsigned long long ind = index;
        while (ind > 1)
        {
            if (curr_index->getNextListMember() == nullptr) { return false; }
            else
            {
                curr_index = curr_index->getNextListMember();
                ind--;
            }
        }

        ListMember<KIniProperty>* prior = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            prior->setNextListMember(new ListMember<KIniProperty>(index, true, nullptr, to_add, nullptr));
        }
        else
        {
            curr_index = curr_index->getNextListMember();
            prior->setNextListMember(new ListMember<KIniProperty>(index, true, nullptr, to_add, curr_index));

            curr_index->setListIndex(curr_index->getListIndex()+1);

            while (curr_index->getNextListMember() != nullptr)
            {
                curr_index = curr_index->getNextListMember();
                curr_index->setListIndex(curr_index->getListIndex()+1);
            }
        }
        return true;
    }
}

// KIniElement Methods
bool KIniElement::insertInstanceAtIndex(unsigned long long index, KIniInstance* to_add)
{
    ListMember<KIniInstance>* curr_index = getInstanceList();

    if (curr_index == nullptr)
    {
        if (index == 0)
        {
            setInstanceList(new ListMember<KIniInstance>(0, false, to_add, nullptr, nullptr));
            return true;
        }
        else { return false; }
    }
    else
    {
        unsigned long long ind = index;
        while (ind > 1)
        {
            if (curr_index->getNextListMember()) { return false; }
            else
            {
                curr_index = curr_index->getNextListMember();
                ind--;
            }
        }

        ListMember<KIniInstance>* prior = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            prior->setNextListMember(new ListMember<KIniInstance>(index, false, to_add, nullptr, nullptr));
        }
        else
        {
            curr_index = curr_index->getNextListMember();
            prior->setNextListMember(new ListMember<KIniInstance>(index, false, to_add, nullptr, curr_index));

            curr_index->setListIndex(curr_index->getListIndex()+1);

            while (curr_index->getNextListMember() != nullptr)
            {
                curr_index = curr_index->getNextListMember();
                curr_index->setListIndex(curr_index->getListIndex()+1);
            }
        }
        return true;
    }
}

bool KIniElement::insertCommentAtIndex(unsigned long long index, KIniComment* to_add)
{
    ListMember<KIniInstance>* curr_index = getInstanceList();

    if (curr_index == nullptr)
    {
        if (index == 0)
        {
            setInstanceList(new ListMember<KIniInstance>(0, true, nullptr, to_add, nullptr));
            return true;
        }
        else { return false; }
    }
    else
    {
        unsigned long long ind = index;
        while (ind > 1)
        {
            if (curr_index->getNextListMember()) { return false; }
            else
            {
                curr_index = curr_index->getNextListMember();
                ind--;
            }
        }

        ListMember<KIniInstance>* prior = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            prior->setNextListMember(new ListMember<KIniInstance>(index, true, nullptr, to_add, nullptr));
        }
        else
        {
            curr_index = curr_index->getNextListMember();
            prior->setNextListMember(new ListMember<KIniInstance>(index, true, nullptr, to_add, curr_index));

            curr_index->setListIndex(curr_index->getListIndex()+1);

            while (curr_index->getNextListMember() != nullptr)
            {
                curr_index = curr_index->getNextListMember();
                curr_index->setListIndex(curr_index->getListIndex()+1);
            }
        }
        return true;
    }
}

// KIniRoot Methods
bool KIniRoot::insertElementAtIndex(unsigned long long index, KIniElement* to_add)
{
    ListMember<KIniElement>* curr_index = getElementList();

    if (curr_index == nullptr)
    {
        if (index == 0)
        {
            setElementList(new ListMember<KIniElement>(0, false, to_add, nullptr, nullptr));
            return true;
        }
        else { return false; }
    }
    else
    {
        unsigned long long ind = index;
        while (ind > 1)
        {
            if (curr_index->getNextListMember()) { return false; }
            else
            {
                curr_index = curr_index->getNextListMember();
                ind--;
            }
        }

        ListMember<KIniElement>* prior = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            prior->setNextListMember(new ListMember<KIniElement>(index, false, to_add, nullptr, nullptr));
        }
        else
        {
            curr_index = curr_index->getNextListMember();
            prior->setNextListMember(new ListMember<KIniElement>(index, false, to_add, nullptr, curr_index));

            curr_index->setListIndex(curr_index->getListIndex()+1);

            while (curr_index->getNextListMember() != nullptr)
            {
                curr_index = curr_index->getNextListMember();
                curr_index->setListIndex(curr_index->getListIndex()+1);
            }
        }
        return true;
    }
}

bool KIniRoot::insertCommentAtIndex(unsigned long long index, KIniComment* to_add)
{
    ListMember<KIniElement>* curr_index = getElementList();

    if (curr_index == nullptr)
    {
        if (index == 0)
        {
            setElementList(new ListMember<KIniElement>(0, true, nullptr, to_add, nullptr));
            return true;
        }
        else { return false; }
    }
    else
    {
        unsigned long long ind = index;
        while (ind > 1)
        {
            if (curr_index->getNextListMember()) { return false; }
            else
            {
                curr_index = curr_index->getNextListMember();
                ind--;
            }
        }

        ListMember<KIniElement>* prior = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            prior->setNextListMember(new ListMember<KIniElement>(index, true, nullptr, to_add, nullptr));
        }
        else
        {
            curr_index = curr_index->getNextListMember();
            prior->setNextListMember(new ListMember<KIniElement>(index, true, nullptr, to_add, curr_index));

            curr_index->setListIndex(curr_index->getListIndex()+1);

            while (curr_index->getNextListMember() != nullptr)
            {
                curr_index = curr_index->getNextListMember();
                curr_index->setListIndex(curr_index->getListIndex()+1);
            }
        }
        return true;
    }
}
