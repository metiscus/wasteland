#include "object.hpp"
#include "utility.hpp"

#include <sstream>

struct ObjectData
{
    ObjectType type;
    uint32_t uid;
    uint32_t weight;
    uint32_t quantity;
    std::string name;
};

Object::Object()
    : data_( new ObjectData() )
{
    data_->uid = 0;
    data_->type = object_Invalid;
    data_->weight = 0;
    data_->quantity = 1;
    data_->name = "(invalid object)";
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
        ret->data_->type = (ObjectType)tmp;

        ss.clear();
        ss<<tokens[1];
        ss>>ret->data_->uid;
        
        ss.clear();
        ss<<tokens[2];
        ss>>ret->data_->weight;

        ss.clear();
        ss<<tokens[3];
        ss>>ret->data_->quantity;
        
        ret->data_->name = tokens[4];
    }
    
    return ret;
}

std::string Object::ToString() const
{
    std::stringstream ss;
    ss<<data_->type<<","<<data_->uid<<","<<data_->weight<<","<<data_->quantity<<","<<data_->name;

    return ss.str();
}

uint32_t Object::GetUID() const
{
    return data_->uid;
}

ObjectType Object::GetType() const
{
    return data_->type;
}

std::string Object::GetName() const
{
    return data_->name;
}

uint32_t Object::GetWeight() const
{
    return data_->weight;
}

uint32_t Object::GetQuantity() const
{
    return data_->quantity;
}

void Object::SetQuantity(uint32_t qty)
{
    data_->quantity = qty;
}

void Object::AddQuantity(uint32_t qty)
{
    data_->quantity += qty;
}
