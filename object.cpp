#include "object.hpp"
#include "utility.hpp"

#include <cassert>
#include <sstream>

std::unordered_map<ObjectId, ObjectPtr> Object::objects_;

Object::Object()
{
    uid_ = 0;
    type_ = object_Invalid;
    weight_ = 0;
    name_ = "(invalid object)";
    sprite_ = 0;
}

ObjectPtr Object::GetObject(ObjectId id)
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

    if(tokens.size() >= 5)
    {
        ss.clear();
        ss<<tokens[0];
        ss>>ret->uid_;

        ret->type_ = StringToType(tokens[1]);

        ss.clear();
        ss<<tokens[2];
        ss>>ret->weight_;

        ss.clear();
        ss<<tokens[3];
        ss>>ret->sprite_;

        ret->name_ = tokens[4];

        uint32_t value = 0;
        for(uint32_t ii = 5; ii<tokens.size()-1; ii+=2)
        {
            ss.clear();
            ss<<tokens[ii+1];
            ss>>value;
            ret->properties_.insert(std::make_pair(tokens[ii], value));
        }

        objects_.insert(std::make_pair(ret->uid_, ret));
    }

    return ret;
}

std::string Object::ToString() const
{
    std::stringstream ss;
    ss<<uid_<<","<<type_<<","<<weight_<<","<<name_<<","<<sprite_;
    for(auto itr : properties_)
    {
        ss<<","<<itr.first<<","<<itr.second;
    }

    return ss.str();
}

Object::Instance Object::CreateInstance(ObjectId id, uint32_t qty)
{
    return Object::Instance(GetObject(id), qty);
}

ObjectType Object::StringToType(const std::string& str)
{
    if(str == "OBJECT_FOOD")
    {
        return object_Food;
    }
    else if(str == "OBJECT_WATER")
    {
        return object_Water;
    }
    else if(str == "OBJECT_WEAPON")
    {
        return object_Weapon;
    }
    else if(str == "OBJECT_ARMOR")
    {
        return object_Armor;
    }
    else
    {
        std::stringstream ss;
        ss<<str;
        uint32_t val = object_Invalid;
        ss>>val;
        return (ObjectType)val;
    }
}

ObjectId Object::GetId() const
{
    return uid_;
}

ObjectType Object::GetType() const
{
    return type_;
}

const std::string& Object::GetName() const
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

uint32_t Object::GetProperty(const std::string& name) const
{
    uint32_t val = 0;
    auto itr = properties_.find(name);
    if(itr!=properties_.end())
    {
        val = itr->second;
    }
    else
    {
        assert(false);
    }
    return val;
}

void Object::SetProperty(const std::string& name, const uint32_t& value)
{
    auto itr = properties_.find(name);
    if(itr!=properties_.end())
    {
        itr->second = value;
    }
    else
    {
        properties_.insert(std::make_pair(name, value));
    }
}

//////////[ Object::Instance ]//////////
Object::Instance::Instance(ObjectPtr obj, uint32_t qty)
    : quantity_(qty)
    , parent_(obj)
{
    uid_ = obj->GetId();
}

ObjectId Object::Instance::GetId() const
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
    if(qty<0 && -qty>(int32_t)quantity_)
    {
        quantity_ = 0;
    }
    else
    {
        quantity_ += qty;
    }
}

const std::string& Object::Instance::GetName() const
{
    return parent_->GetName();
}
