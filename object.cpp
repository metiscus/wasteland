#include "object.hpp"
#include "utility.hpp"

#include <sstream>

Object::Object()
{
    uid_ = 0;
    type_ = object_Invalid;
    weight_ = 0;
    quantity_ = 1;
    name_ = "(invalid object)";
    sprite_ = 0;
}

ObjectPtr Object::BuildFromString(const std::string& str)
{
    auto ret = std::make_shared<Object>();
    std::vector<std::string> tokens = TokenizeString(str);
    std::stringstream ss;
    
    if(tokens.size() == 5)
    {
        ss<<tokens[0];
        uint32_t tmp;
        ss>>tmp;
        ret->type_ = (ObjectType)tmp;

        ss.clear();
        ss<<tokens[1];
        ss>>ret->uid_;
        
        ss.clear();
        ss<<tokens[2];
        ss>>ret->weight_;

        ss.clear();
        ss<<tokens[3];
        ss>>ret->quantity_;
        
        ret->name_ = tokens[4];
    }
    
    return ret;
}

std::string Object::ToString() const
{
    std::stringstream ss;
    ss<<type_<<","<<uid_<<","<<weight_<<","<<quantity_<<","<<name_<<","<<sprite_;

    return ss.str();
}

uint32_t Object::GetUID() const
{
    return uid_;
}

ObjectType Object::GetType() const
{
    return type_;
}

std::string Object::GetName() const
{
    return name_;
}

uint32_t Object::GetWeight() const
{
    return weight_;
}

uint32_t Object::GetQuantity() const
{
    return quantity_;
}

uint32_t Object::GetSprite() const
{
    return sprite_;
}

void Object::SetQuantity(uint32_t qty)
{
    quantity_ = qty;
}

void Object::AddQuantity(uint32_t qty)
{
    quantity_ += qty;
}
