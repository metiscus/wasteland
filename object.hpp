#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <string>

enum ObjectType
{
    object_Invalid,
    object_Food,
    object_Water,
    object_Weapon,
    object_Armor,
    
    object_Count
};

struct ObjectData;

class Object;
typedef std::shared_ptr<Object> ObjectPtr;

class Object final
{
private:
    ObjectType type_;
    uint32_t uid_;
    uint32_t weight_;
    uint32_t quantity_;
    std::string name_;
    uint32_t sprite_;

public:
    Object();
    static ObjectPtr BuildFromString(const std::string& str);
    std::string ToString() const;
    
    uint32_t GetUID() const;
    ObjectType GetType() const;
    std::string GetName() const;
    uint32_t GetWeight() const;
    uint32_t GetQuantity() const;
    uint32_t GetSprite() const;
    
    void SetQuantity(uint32_t qty);
    void AddQuantity(uint32_t qty);
};

#endif
