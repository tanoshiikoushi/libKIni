#include "KIni.h"

// toString Methods
std::string KIniComment::toString()
{
    return "// " + getValue() + "\n";
}

std::string KIniEvent::toString()
{
    std::string out = "  ";
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
    return out;
}

std::string KIniProperty::toString()
{
    if (isEvent())
    {
        return getPropertyName() + " = " + getPropertyValue() + "\n" + getEvent()->toString();
    }
    else if (isMultivalue())
    {
        std::string out = getPropertyName();
        std::string* multis = getMultivalues();
        if (isDoubleSpace())
        {
            for (uint8_t i = 0; i < getMultivalueCount()-1; i++)
            {
                out.append("  " + multis[i] + ",");
            }
            out.append("  " + multis[getMultivalueCount()-1] + "\n");
        }
        else
        {
            for (uint8_t j = 0; j < getMultivalueCount(); j++)
            {
                out.append(" " + multis[j]);
            }
            out.append("\n");
        }

        return out;
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
    return out;
}

std::string KIniRoot::toString()
{
    std::string out = "";
    ListMember<KIniElement>* index = getElementList();
    bool first_elem = true;

    while (index != nullptr)
    {
        if (index->isComment())
        {
            out.append(index->getCurrentComment()->toString());
        }
        else
        {
            if (first_elem) { out.append("\n\n"); first_elem = false; }
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

// Extra Methods
std::string* generateMultivalueDefault()
{
    std::string* out = new std::string[KINI_PROPERTY_MULTIVALUE_MAX];
    for (uint8_t i = 0; i < KINI_PROPERTY_MULTIVALUE_MAX; i++)
    {
        out[i] = "";
    }
    return out;
}

// Parsing Methods
bool parseKIni(KIniRoot* root, std::string file_path)
{
    std::fstream file;
    file.open(file_path.c_str(), std::ios::in);

    if (file.fail())
    {
        return false;
    }

    // 0 = Root, 1 = Element, 2 = Instance, 3 = Property, 4 = Event
    uint8_t zone = 0;
    char* read = new char[KINI_BUFFER_IN_SIZE];
    std::string input;
    std::string key;
    std::string value;

    bool make_new_instance = false;
    bool last_was_event = false;

    KIniElement* curr_elem = nullptr;
    KIniInstance* curr_instance = nullptr;
    KIniProperty* curr_property = nullptr;
    KIniEvent* curr_event = nullptr;

    while (!file.eof())
    {
        file.getline(read, KINI_BUFFER_IN_SIZE, '\n');
        input.assign(read);

        if (input.length() > 0)
        {
            if (input.at(input.length()-1) == '\r')
            {
                input = input.substr(0, input.length()-1);
            }
        }

        if (input.empty())
        {
            last_was_event = false;
            switch (zone)
            {
            case 1:
            case 2:
                make_new_instance = true;
                break;
            default:
                break;
            }
        }
        else if (input.substr(0, 2) == "//")
        {
            last_was_event = false;
            make_new_instance = false;

            if (input.size() == 2) // empty comment
            {
                input = "// ";
            }

            switch (zone)
            {
            case 0:
                root->appendComment(new KIniComment(input.substr(3)));
                break;
            case 1:
                curr_elem->appendComment(new KIniComment(input.substr(3)));
                break;
            case 2:
                curr_instance->appendComment(new KIniComment(input.substr(3)));
                break;
            case 4:
                curr_event->appendComment(new KIniComment(input.substr(3)));
                break;
            default:
                break;
            }
        }
        else if (input.substr(0, 2) == "  ")
        {
            make_new_instance = false;

            if (!last_was_event)
            {
                curr_event = new KIniEvent(nullptr);
                curr_property->setEvent(curr_event);
                curr_property->setIsEvent(true);
            }

            curr_property = new KIniProperty(false, false, false, input.substr(2, input.find("=") - 1), input.substr(input.find("=") + 2), nullptr, nullptr, 0);
            curr_event->appendProperty(curr_property);
            last_was_event = true;
        }
        else if (input.substr(0, 4) == "name")
        {
            last_was_event = false;
            make_new_instance = false;
            // this is a new element
            curr_elem = new KIniElement(input.substr(5), nullptr);
            root->appendElement(curr_elem);
            zone = 1;
        }
        else
        {
            // this is a property and potentially a new instance
            if (make_new_instance)
            {
                make_new_instance = false;

                // this is a new instance
                curr_instance = new KIniInstance(nullptr);
                curr_elem->appendInstance(curr_instance);
                zone = 2;
            }

            // multivalue recognition
            if (input.find("=") == std::string::npos)
            {
                last_was_event = false;
                std::string* multi = generateMultivalueDefault();
                std::string curr_value = "";

                uint16_t first_space = input.find(" ");
                if (first_space == (uint16_t)std::string::npos)
                {
                    // These are dumbfuck anomalous things that I don't think should be there at all but whatever lol
                    curr_property = new KIniProperty(false, true, false, input, "", nullptr, nullptr, 0);
                }
                else
                {
                    std::string prop = input.substr(0, first_space);
                    bool double_space = (input.at(first_space + 1) ==  ' ' ? true : false);
                    uint16_t curr_search_spot = 0;
                    uint8_t curr_ind = 0;
                    uint16_t occurence = 0;

                    if (double_space)
                    {
                        curr_search_spot = first_space + 2;
                        occurence = input.find("  ", curr_search_spot);

                        while (occurence != (uint16_t)std::string::npos)
                        {
                            multi[curr_ind] = input.substr(curr_search_spot, occurence-curr_search_spot-1); // this removes the comma

                            curr_search_spot = occurence + 2;
                            occurence = input.find("  ", curr_search_spot);
                            curr_ind++;
                        }

                        multi[curr_ind] = input.substr(curr_search_spot);
                        curr_ind++;

                        curr_property = new KIniProperty(false, true, true, prop, "", nullptr, multi, curr_ind);
                    }
                    else
                    {
                        curr_search_spot = first_space + 1;
                        occurence = input.find(" ", curr_search_spot);

                        while (occurence != (uint16_t)std::string::npos)
                        {
                            multi[curr_ind] = input.substr(curr_search_spot, occurence-curr_search_spot);

                            curr_search_spot = occurence + 1;
                            occurence = input.find(" ", curr_search_spot);
                            curr_ind++;
                        }

                        multi[curr_ind] = input.substr(curr_search_spot);
                        curr_ind++;

                        curr_property = new KIniProperty(false, true, false, prop, "", nullptr, multi, curr_ind);
                    }
                }
                curr_instance->appendProperty(curr_property);
            }
            else
            {
                last_was_event = false;
                zone = 2;
                curr_property = new KIniProperty(false, false, false, input.substr(0, input.find("=") - 1), input.substr(input.find("=") + 2), nullptr, nullptr, 0);
                curr_instance->appendProperty(curr_property);
            }
        }
    }

    file.close();
    delete read;

    return true;
}
