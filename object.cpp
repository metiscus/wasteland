#include "object.hpp"
#include "utility.hpp"

#include <sstream>

std::unordered_map<uint32_t, ObjectPtr> Object::objects_;

Object::Object()
{
    uid_ = 0;
    type_ = object_Invalid;
    weight_ = 0;
    name_ = "(invalid object)";
    sprite_ = 0;
}

ObjectPtr Object::GetObject(uint32_t id)
{
    ObjectPtr ret;
    auto itr = objects_.find(id);
    if(itr != objects_.end())
    {
        ret = itr->second;
    }
    return ret;
}

ObjectPtr Object::BuildFromString(const std::string& str)
{
    std::shared_ptr<Object> ret (new Object ());
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
        ss>>ret->sprite_;
        
        ret->name_ = tokens[4];
        
        objects_.insert(std::make_pair(ret->uid_, ret));
    }
    
    return ret;
}

std::string Object::ToString() const
{
    std::stringstream ss;
    ss<<type_<<","<<uid_<<","<<weight_<<","<<name_<<","<<sprite_;

    return ss.str();
}

Object::Instance Object::CreateInstance(ObjectPtr obj, uint32_t qty)
{
    return Object::Instance(obj, qty);
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

uint32_t Object::GetSprite() const
{
    return sprite_;
}

//////////[ Object::Instance ]//////////
Object::Instance::Instance(ObjectPtr obj, uint32_t qty)
    : quantity_(qty)
    , parent_(obj)
{
    uid_ = obj->GetUID();
}

uint32_t Object::Instance::GetUID() const
{
    return uid_;
}

uint32_t Object::Instance::GetQuantity() const
{
    return quantity_;
}

ObjectPtr Object::Instance::GetParent() const
{
    return parent_;
}

ObjectPtr Object::Instance::GetParent()
{
    return parent_;
}

void Object::Instance::SetQuantity(uint32_t qty)
{
    quantity_ = qty;
}

void Object::Instance::ChangeQuantity(int32_t qty)
{
    if(qty<0 && -qty>quantity_)
    {
        quantity_ = 0;
    }
    else
    {
        quantity_ += qty;
    }
}
