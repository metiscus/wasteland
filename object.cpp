#include "object.hpp"
#include "utility.hpp"

#include <sstream>

Object::Object()
{
    uid = 0;
    type = object_Invalid;
    weight = 0;
    quantity = 1;
    name = "(invalid object)";
}

std::shared_ptr<Object> Object::BuildFromString(const std::string& str)
{
    auto ret = std::make_shared<Object>();
    std::vector<std::string> tokens = TokenizeString(str);
    std::stringstream ss;
    
    if(tokens.size() == 5)
    {
        ss<<tokens[0];
        uint32_t tmp;
        ss>>tmp;
        ret->type = (ObjectType)tmp;

        ss.clear();
        ss<<tokens[1];
        ss>>ret->uid;
        
        ss.clear();
        ss<<tokens[2];
        ss>>ret->weight;

        ss.clear();
        ss<<tokens[3];
        ss>>ret->quantity;
        
        ret->name = tokens[4];
    }
    
    return ret;
}

std::string Object::ToString() const
{
    std::stringstream ss;
    ss<<type<<","<<uid<<","<<weight<<","<<quantity<<","<<name;

    return ss.str();
}

uint32_t Object::GetUID() const
{
    return uid;
}

ObjectType Object::GetType() const
{
    return type;
}

std::string Object::GetName() const
{
    return name;
}

uint32_t Object::GetWeight() const
{
    return weight;
}

uint32_t Object::GetQuantity() const
{
    return quantity;
}

void Object::SetQuantity(uint32_t qty)
{
    quantity = qty;
}

void Object::AddQuantity(uint32_t qty)
{
    quantity += qty;
}
