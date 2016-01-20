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

class Object final
{
private:
    std::unique_ptr<ObjectData> data_;
    
public:
    Object();
    static std::shared_ptr<Object> BuildFromString(const std::string& str);
    std::string ToString() const;
    
    uint32_t GetUID() const;
    ObjectType GetType() const;
    std::string GetName() const;
    uint32_t GetWeight() const;
    uint32_t GetQuantity() const;
    
    void SetQuantity(uint32_t qty);
    void AddQuantity(uint32_t qty);
};

#endif
