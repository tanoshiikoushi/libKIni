#ifndef KINI_H_INCLUDED

#include <string>

class KIniComment
{
    private:
        std::string value;

    public:
        //Getters and Setters
        std::string getValue() { return value; }

        void setValue(std::string value) { this->value = value; }

        //Constructor
        KIniComment(std::string value)
        {
            setValue(value);
        }

        //Methods
        std::string toString();
};

template <typename containing_type> class ListMember
{
    private:
        uint32_t list_index;
        bool is_comment;
        containing_type* current_contained;
        KIniComment* current_comment;
        ListMember<containing_type>* next_list_member;

    public:
        //Getters and Setters
        uint32_t getListIndex() { return list_index; }
        bool isComment() { return is_comment; }
        containing_type* getCurrentContained() { return current_contained; }
        KIniComment* getCurrentComment() { return current_comment; }
        ListMember<containing_type>* getNextListMember() { return next_list_member; }

        void setListIndex(uint32_t list_index) { this->list_index = list_index; }
        void setIsComment(bool is_comment) { this->is_comment = is_comment; }
        void setCurrentContained(containing_type* contained) { this->current_contained = contained; }
        void setCurrentComment(KIniComment* comment) { this->current_comment = comment; }
        void setNextListMember(ListMember<containing_type>* list_member) { this->next_list_member = list_member; }

        //Constructor
        ListMember(uint32_t list_index, bool is_comment, containing_type* contained, KIniComment* current_comment, ListMember<containing_type>* next_list_member)
        {
            setListIndex(list_index);
            setIsComment(is_comment);
            setCurrentContained(contained);
            setCurrentComment(current_comment);
            setNextListMember(next_list_member);
        }

        //Destructor
        ~ListMember()
        {
            if (is_comment)
            {
                delete current_comment;
            }
            else
            {
                delete current_contained;
            }

            if (next_list_member != nullptr)
            {
                delete next_list_member;
            }
        }
};

class KIniProperty
{
    private:
        std::string name;
        std::string value;

    public:
        //Getters and Setters
        std::string getPropertyName() { return name; }
        std::string getPropertyValue() { return value; }
        void setPropertyName(std::string name) { this->name = name; }
        void setPropertyValue(std::string value) { this->value = value; }

        //Constructor
        KIniProperty(std::string property_name, std::string property_value)
        {
            setPropertyName(property_name);
            setPropertyValue(property_value);
        }

        //Methods
        std::string toString();
};

class KIniInstance
{
    private:
        ListMember<KIniProperty>* property_list;

    public:
        //Getters and Setters
        ListMember<KIniProperty>* getPropertyList() { return property_list; }

        void setPropertyList(ListMember<KIniProperty>* property_list) { this->property_list = property_list; }

        //Constructor
        KIniInstance(ListMember<KIniProperty>* property_list)
        {
            setPropertyList(property_list);
        }

        //Destructor
        ~KIniInstance()
        {
            if (property_list != nullptr)
            {
                delete property_list;
            }
        }

        //Methods
        bool insertPropertyAtIndex(uint32_t index, KIniProperty* to_add);
        bool insertCommentAtIndex(uint32_t index, KIniComment* to_add);

        void appendProperty(KIniProperty* to_add);
        void appendComment(KIniComment* to_add);

        ListMember<KIniProperty>* getMemberAtIndex(uint32_t index);

        bool deleteMemberAtIndex(uint32_t index);

        KIniProperty* queryPropertyByName(std::string prop_name);
        KIniComment* queryCommentByString(std::string comm_string);

        std::string toString();
};

class KIniElement
{
    private:
        std::string name;
        ListMember<KIniInstance>* instance_list;

    public:
        //Getters and Setters
        std::string getElementName() { return name; }
        ListMember<KIniInstance>* getInstanceList() { return instance_list; }

        void setElementName(std::string name) { this->name = name; }
        void setInstanceList(ListMember<KIniInstance>* instance_list) { this->instance_list = instance_list; }

        //Constructor
        KIniElement(std::string element_name, ListMember<KIniInstance>* instance_list)
        {
            setElementName(element_name);
            setInstanceList(instance_list);
        }

        //Destructor
        ~KIniElement()
        {
            if (instance_list != nullptr)
            {
                delete instance_list;
            }
        }

        //Methods
        bool insertInstanceAtIndex(uint32_t index, KIniInstance* to_add);
        bool insertCommentAtIndex(uint32_t index, KIniComment* to_add);

        void appendInstance(KIniInstance* to_add);
        void appendComment(KIniComment* to_add);

        ListMember<KIniInstance>* getMemberAtIndex(uint32_t index);

        bool deleteMemberAtIndex(uint32_t index);

        std::string toString();
};

class KIniRoot
{
    private:
        ListMember<KIniElement>* element_list;

    public:
        //Getters and Setters
        ListMember<KIniElement>* getElementList() { return element_list; }

        void setElementList(ListMember<KIniElement>* element_list) { this->element_list = element_list; }

        //Constructor
        KIniRoot(ListMember<KIniElement>* element_list)
        {
            setElementList(element_list);
        }

        //Destructor
        ~KIniRoot()
        {
            if (element_list != nullptr)
            {
                delete element_list;
            }
        }

        //Methods
        bool insertElementAtIndex(uint32_t index, KIniElement* to_add);
        bool insertCommentAtIndex(uint32_t index, KIniComment* to_add);

        void appendElement(KIniElement* to_add);
        void appendComment(KIniComment* to_add);

        ListMember<KIniElement>* getMemberAtIndex(uint32_t index);

        bool deleteMemberAtIndex(uint32_t index);

        KIniElement* queryElementByName(std::string elem_name);
        KIniComment* queryCommentByString(std::string comm_string);

        std::string toString();
};

#define KINI_H_INCLUDED

#endif // KINI_H_INCLUDED
