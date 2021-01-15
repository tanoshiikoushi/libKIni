#include "KIni.h"
#include <iostream>

// toString Methods
std::string KIniComment::toString()
{
    return "// " + getValue() + "\n";
}

std::string KIniEvent::toString()
{
    std::string out = "";
    ListMember<KIniProperty>* index = getPropertyList();

    while (index != nullptr)
    {
        if (index->isComment())
        {
            out.append("  " + index->getCurrentComment()->toString());
        }
        else
        {
            out.append("  " + index->getCurrentContained()->toString());
        }
        index = index->getNextListMember();
    }
    out.append("\n");
    return out;
}

std::string KIniProperty::toString()
{
    if (isEvent())
    {
        return getPropertyName() + " = event\n" + getEvent()->toString();
    }
    else
    {
        return getPropertyName() + " = " + getPropertyValue() + "\n";
    }
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
    out.append("\n");
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

// KIniEvent Methods
bool KIniEvent::insertPropertyAtIndex(uint32_t index, KIniProperty* to_add)
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
        uint32_t ind = index;
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

bool KIniEvent::insertCommentAtIndex(uint32_t index, KIniComment* to_add)
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
        uint32_t ind = index;
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

void KIniEvent::appendProperty(KIniProperty* to_add)
{
    ListMember<KIniProperty>* curr_index = getPropertyList();
    uint32_t curr_id = 0;

    if (curr_index == nullptr)
    {
        setPropertyList(new ListMember<KIniProperty>(0, false, to_add, nullptr, nullptr));
    }
    else
    {
        while (curr_index->getNextListMember() != nullptr)
        {
            curr_index = curr_index->getNextListMember();
            curr_id++;
        }

        curr_index->setNextListMember(new ListMember<KIniProperty>(curr_id+1, false, to_add, nullptr, nullptr));
    }
}

void KIniEvent::appendComment(KIniComment* to_add)
{
    ListMember<KIniProperty>* curr_index = getPropertyList();
    uint32_t curr_id = 0;

    if (curr_index == nullptr)
    {
        setPropertyList(new ListMember<KIniProperty>(0, true, nullptr, to_add, nullptr));
    }
    else
    {
        while (curr_index->getNextListMember() != nullptr)
        {
            curr_index = curr_index->getNextListMember();
            curr_id++;
        }

        curr_index->setNextListMember(new ListMember<KIniProperty>(curr_id+1, true, nullptr, to_add, nullptr));
    }
}

ListMember<KIniProperty>* KIniEvent::getMemberAtIndex(uint32_t index)
{
    uint32_t ind = 0;
    ListMember<KIniProperty>* curr = getPropertyList();
    while (ind < index)
    {
        if (curr->getNextListMember() != nullptr)
        {
            ind++;
            curr = curr->getNextListMember();
        }
        else
        {
            return nullptr;
        }
    }

    return curr;
}

bool KIniEvent::deleteMemberAtIndex(uint32_t index)
{
    ListMember<KIniProperty>* curr_index = getPropertyList();

    if (curr_index == nullptr)
    {
        return false;
    }
    else if (index == 0)
    {
        ListMember<KIniProperty>* to_delete = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            setPropertyList(nullptr);
            return true;
        }

        curr_index = to_delete->getNextListMember();
        setPropertyList(curr_index);

        to_delete->setNextListMember(nullptr);
        delete to_delete;

        curr_index->setListIndex(curr_index->getListIndex()-1);
        while (curr_index->getNextListMember() != nullptr)
        {
            curr_index = curr_index->getNextListMember();
            curr_index->setListIndex(curr_index->getListIndex()-1);
        }

        return true;
    }
    else
    {
        uint32_t ind = 0;

        while (ind < (index-1))
        {
            if (curr_index->getNextListMember() == nullptr) { return false; }
            curr_index = curr_index->getNextListMember();
            ind++;
        }

        ListMember<KIniProperty>* prior = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            return false;
        }
        else
        {
            ListMember<KIniProperty>* to_delete = curr_index->getNextListMember();

            if (to_delete->getNextListMember() == nullptr)
            {
                prior->setNextListMember(nullptr);
                delete to_delete;

                return true;
            }
            else
            {
                curr_index = to_delete->getNextListMember();

                prior->setNextListMember(curr_index);
                to_delete->setNextListMember(nullptr);
                delete to_delete;
                curr_index->setListIndex(curr_index->getListIndex()-1);

                while (curr_index->getNextListMember() != nullptr)
                {
                    curr_index = curr_index->getNextListMember();
                    curr_index->setListIndex(curr_index->getListIndex()-1);
                }

                return true;
            }
        }
    }
}

KIniProperty* KIniEvent::queryPropertyByName(std::string prop_name)
{
    ListMember<KIniProperty>* curr = getPropertyList();

    while (curr != nullptr)
    {
        if (!curr->isComment())
        {
            if (prop_name.compare(curr->getCurrentContained()->getPropertyName()) == 0)
            {
                return curr->getCurrentContained();
            }
        }
        curr = curr->getNextListMember();
    }
    return nullptr;
}

KIniComment* KIniEvent::queryCommentByString(std::string comm_name)
{
    ListMember<KIniProperty>* curr = getPropertyList();

    while (curr != nullptr)
    {
        if (curr->isComment())
        {
            if (curr->getCurrentComment()->getValue().find(comm_name) != std::string::npos)
            {
                return curr->getCurrentComment();
            }
        }
        curr = curr->getNextListMember();
    }
    return nullptr;
}

// KIniInstance Methods
bool KIniInstance::insertPropertyAtIndex(uint32_t index, KIniProperty* to_add)
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
        uint32_t ind = index;
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

bool KIniInstance::insertCommentAtIndex(uint32_t index, KIniComment* to_add)
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
        uint32_t ind = index;
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

void KIniInstance::appendProperty(KIniProperty* to_add)
{
    ListMember<KIniProperty>* curr_index = getPropertyList();
    uint32_t curr_id = 0;

    if (curr_index == nullptr)
    {
        setPropertyList(new ListMember<KIniProperty>(0, false, to_add, nullptr, nullptr));
    }
    else
    {
        while (curr_index->getNextListMember() != nullptr)
        {
            curr_index = curr_index->getNextListMember();
            curr_id++;
        }

        curr_index->setNextListMember(new ListMember<KIniProperty>(curr_id+1, false, to_add, nullptr, nullptr));
    }
}

void KIniInstance::appendComment(KIniComment* to_add)
{
    ListMember<KIniProperty>* curr_index = getPropertyList();
    uint32_t curr_id = 0;

    if (curr_index == nullptr)
    {
        setPropertyList(new ListMember<KIniProperty>(0, true, nullptr, to_add, nullptr));
    }
    else
    {
        while (curr_index->getNextListMember() != nullptr)
        {
            curr_index = curr_index->getNextListMember();
            curr_id++;
        }

        curr_index->setNextListMember(new ListMember<KIniProperty>(curr_id+1, true, nullptr, to_add, nullptr));
    }
}

ListMember<KIniProperty>* KIniInstance::getMemberAtIndex(uint32_t index)
{
    uint32_t ind = 0;
    ListMember<KIniProperty>* curr = getPropertyList();
    while (ind < index)
    {
        if (curr->getNextListMember() != nullptr)
        {
            ind++;
            curr = curr->getNextListMember();
        }
        else
        {
            return nullptr;
        }
    }

    return curr;
}

bool KIniInstance::deleteMemberAtIndex(uint32_t index)
{
    ListMember<KIniProperty>* curr_index = getPropertyList();

    if (curr_index == nullptr)
    {
        return false;
    }
    else if (index == 0)
    {
        ListMember<KIniProperty>* to_delete = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            setPropertyList(nullptr);
            return true;
        }

        curr_index = to_delete->getNextListMember();
        setPropertyList(curr_index);

        to_delete->setNextListMember(nullptr);
        delete to_delete;

        curr_index->setListIndex(curr_index->getListIndex()-1);
        while (curr_index->getNextListMember() != nullptr)
        {
            curr_index = curr_index->getNextListMember();
            curr_index->setListIndex(curr_index->getListIndex()-1);
        }

        return true;
    }
    else
    {
        uint32_t ind = 0;

        while (ind < (index-1))
        {
            if (curr_index->getNextListMember() == nullptr) { return false; }
            curr_index = curr_index->getNextListMember();
            ind++;
        }

        ListMember<KIniProperty>* prior = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            return false;
        }
        else
        {
            ListMember<KIniProperty>* to_delete = curr_index->getNextListMember();

            if (to_delete->getNextListMember() == nullptr)
            {
                prior->setNextListMember(nullptr);
                delete to_delete;

                return true;
            }
            else
            {
                curr_index = to_delete->getNextListMember();

                prior->setNextListMember(curr_index);
                to_delete->setNextListMember(nullptr);
                delete to_delete;
                curr_index->setListIndex(curr_index->getListIndex()-1);

                while (curr_index->getNextListMember() != nullptr)
                {
                    curr_index = curr_index->getNextListMember();
                    curr_index->setListIndex(curr_index->getListIndex()-1);
                }

                return true;
            }
        }
    }
}

KIniProperty* KIniInstance::queryPropertyByName(std::string prop_name)
{
    ListMember<KIniProperty>* curr = getPropertyList();

    while (curr != nullptr)
    {
        if (!curr->isComment())
        {
            if (prop_name.compare(curr->getCurrentContained()->getPropertyName()) == 0)
            {
                return curr->getCurrentContained();
            }
        }
        curr = curr->getNextListMember();
    }
    return nullptr;
}

KIniComment* KIniInstance::queryCommentByString(std::string comm_name)
{
    ListMember<KIniProperty>* curr = getPropertyList();

    while (curr != nullptr)
    {
        if (curr->isComment())
        {
            if (curr->getCurrentComment()->getValue().find(comm_name) != std::string::npos)
            {
                return curr->getCurrentComment();
            }
        }
        curr = curr->getNextListMember();
    }
    return nullptr;
}

// KIniElement Methods
bool KIniElement::insertInstanceAtIndex(uint32_t index, KIniInstance* to_add)
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
        uint32_t ind = index;
        while (ind > 1)
        {
            if (curr_index->getNextListMember() == nullptr) { return false; }
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

bool KIniElement::insertCommentAtIndex(uint32_t index, KIniComment* to_add)
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
        uint32_t ind = index;
        while (ind > 1)
        {
            if (curr_index->getNextListMember() == nullptr) { return false; }
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

void KIniElement::appendInstance(KIniInstance* to_add)
{
    ListMember<KIniInstance>* curr_index = getInstanceList();
    uint32_t curr_id = 0;

    if (curr_index == nullptr)
    {
        setInstanceList(new ListMember<KIniInstance>(0, false, to_add, nullptr, nullptr));
    }
    else
    {
        while (curr_index->getNextListMember() != nullptr)
        {
            curr_index = curr_index->getNextListMember();
            curr_id++;
        }

        curr_index->setNextListMember(new ListMember<KIniInstance>(curr_id+1, false, to_add, nullptr, nullptr));
    }
}

void KIniElement::appendComment(KIniComment* to_add)
{
    ListMember<KIniInstance>* curr_index = getInstanceList();
    uint32_t curr_id = 0;

    if (curr_index == nullptr)
    {
        setInstanceList(new ListMember<KIniInstance>(0, true, nullptr, to_add, nullptr));
    }
    else
    {
        while (curr_index->getNextListMember() != nullptr)
        {
            curr_index = curr_index->getNextListMember();
            curr_id++;
        }

        curr_index->setNextListMember(new ListMember<KIniInstance>(curr_id+1, true, nullptr, to_add, nullptr));
    }
}

ListMember<KIniInstance>* KIniElement::getMemberAtIndex(uint32_t index)
{
    uint32_t ind = 0;
    ListMember<KIniInstance>* curr = getInstanceList();
    while (ind < index)
    {
        if (curr->getNextListMember() != nullptr)
        {
            ind++;
            curr = curr->getNextListMember();
        }
        else
        {
            return nullptr;
        }
    }

    return curr;
}

bool KIniElement::deleteMemberAtIndex(uint32_t index)
{
    ListMember<KIniInstance>* curr_index = getInstanceList();

    if (curr_index == nullptr)
    {
        return false;
    }
    else if (index == 0)
    {
        ListMember<KIniInstance>* to_delete = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            setInstanceList(nullptr);
            return true;
        }

        curr_index = to_delete->getNextListMember();
        setInstanceList(curr_index);

        to_delete->setNextListMember(nullptr);
        delete to_delete;

        curr_index->setListIndex(curr_index->getListIndex()-1);
        while (curr_index->getNextListMember() != nullptr)
        {
            curr_index = curr_index->getNextListMember();
            curr_index->setListIndex(curr_index->getListIndex()-1);
        }

        return true;
    }
    else
    {
        uint32_t ind = 0;

        while (ind < (index-1))
        {
            if (curr_index->getNextListMember() == nullptr) { return false; }
            curr_index = curr_index->getNextListMember();
            ind++;
        }

        ListMember<KIniInstance>* prior = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            return false;
        }
        else
        {
            ListMember<KIniInstance>* to_delete = curr_index->getNextListMember();

            if (to_delete->getNextListMember() == nullptr)
            {
                prior->setNextListMember(nullptr);
                delete to_delete;

                return true;
            }
            else
            {
                curr_index = to_delete->getNextListMember();

                prior->setNextListMember(curr_index);
                to_delete->setNextListMember(nullptr);
                delete to_delete;
                curr_index->setListIndex(curr_index->getListIndex()-1);

                while (curr_index->getNextListMember() != nullptr)
                {
                    curr_index = curr_index->getNextListMember();
                    curr_index->setListIndex(curr_index->getListIndex()-1);
                }

                return true;
            }
        }
    }
}

// KIniRoot Methods
bool KIniRoot::insertElementAtIndex(uint32_t index, KIniElement* to_add)
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
        uint32_t ind = index;
        while (ind > 1)
        {
            if (curr_index->getNextListMember() == nullptr) { return false; }
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

bool KIniRoot::insertCommentAtIndex(uint32_t index, KIniComment* to_add)
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
        uint32_t ind = index;
        while (ind > 1)
        {
            if (curr_index->getNextListMember() == nullptr) { return false; }
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

void KIniRoot::appendElement(KIniElement* to_add)
{
    ListMember<KIniElement>* curr_index = getElementList();
    uint32_t curr_id = 0;

    if (curr_index == nullptr)
    {
        setElementList(new ListMember<KIniElement>(0, false, to_add, nullptr, nullptr));
    }
    else
    {
        while (curr_index->getNextListMember() != nullptr)
        {
            curr_index = curr_index->getNextListMember();
            curr_id++;
        }

        curr_index->setNextListMember(new ListMember<KIniElement>(curr_id+1, false, to_add, nullptr, nullptr));
    }
}

void KIniRoot::appendComment(KIniComment* to_add)
{
    ListMember<KIniElement>* curr_index = getElementList();
    uint32_t curr_id = 0;

    if (curr_index == nullptr)
    {
        setElementList(new ListMember<KIniElement>(0, true, nullptr, to_add, nullptr));
    }
    else
    {
        while (curr_index->getNextListMember() != nullptr)
        {
            curr_index = curr_index->getNextListMember();
            curr_id++;
        }

        curr_index->setNextListMember(new ListMember<KIniElement>(curr_id+1, true, nullptr, to_add, nullptr));
    }
}

ListMember<KIniElement>* KIniRoot::getMemberAtIndex(uint32_t index)
{
    uint32_t ind = 0;
    ListMember<KIniElement>* curr = getElementList();
    while (ind < index)
    {
        if (curr->getNextListMember() != nullptr)
        {
            ind++;
            curr = curr->getNextListMember();
        }
        else
        {
            return nullptr;
        }
    }

    return curr;
}

bool KIniRoot::deleteMemberAtIndex(uint32_t index)
{
    ListMember<KIniElement>* curr_index = getElementList();

    if (curr_index == nullptr)
    {
        return false;
    }
    else if (index == 0)
    {
        ListMember<KIniElement>* to_delete = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            setElementList(nullptr);
            return true;
        }

        curr_index = to_delete->getNextListMember();
        setElementList(curr_index);

        to_delete->setNextListMember(nullptr);
        delete to_delete;

        curr_index->setListIndex(curr_index->getListIndex()-1);
        while (curr_index->getNextListMember() != nullptr)
        {
            curr_index = curr_index->getNextListMember();
            curr_index->setListIndex(curr_index->getListIndex()-1);
        }

        return true;
    }
    else
    {
        uint32_t ind = 0;

        while (ind < (index-1))
        {
            if (curr_index->getNextListMember() == nullptr) { return false; }
            curr_index = curr_index->getNextListMember();
            ind++;
        }

        ListMember<KIniElement>* prior = curr_index;

        if (curr_index->getNextListMember() == nullptr)
        {
            return false;
        }
        else
        {
            ListMember<KIniElement>* to_delete = curr_index->getNextListMember();

            if (to_delete->getNextListMember() == nullptr)
            {
                prior->setNextListMember(nullptr);
                delete to_delete;

                return true;
            }
            else
            {
                curr_index = to_delete->getNextListMember();

                prior->setNextListMember(curr_index);
                to_delete->setNextListMember(nullptr);
                delete to_delete;
                curr_index->setListIndex(curr_index->getListIndex()-1);

                while (curr_index->getNextListMember() != nullptr)
                {
                    curr_index = curr_index->getNextListMember();
                    curr_index->setListIndex(curr_index->getListIndex()-1);
                }

                return true;
            }
        }
    }
}

KIniElement* KIniRoot::queryElementByName(std::string elem_name)
{
    ListMember<KIniElement>* curr = getElementList();

    while (curr != nullptr)
    {
        if (!curr->isComment())
        {
            if (elem_name.compare(curr->getCurrentContained()->getElementName()) == 0)
            {
                return curr->getCurrentContained();
            }
        }
        curr = curr->getNextListMember();
    }
    return nullptr;
}

KIniComment* KIniRoot::queryCommentByString(std::string comm_name)
{
    ListMember<KIniElement>* curr = getElementList();

    while (curr != nullptr)
    {
        if (curr->isComment())
        {
            if (curr->getCurrentComment()->getValue().find(comm_name) != std::string::npos)
            {
                return curr->getCurrentComment();
            }
        }
        curr = curr->getNextListMember();
    }
    return nullptr;
}
