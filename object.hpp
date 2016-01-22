#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

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
    std::string name_;
    uint32_t sprite_;

    static std::unordered_map<uint32_t, ObjectPtr> objects_;
    Object();

public:
    class Instance
    {
        uint32_t uid_;
        uint32_t quantity_;
        ObjectPtr parent_;

    public:
        Instance(ObjectPtr obj, uint32_t qty = 1);
        
        uint32_t GetUID() const;
        uint32_t GetQuantity() const;
        
        ObjectPtr GetParent() const;
        ObjectPtr GetParent();

        void SetQuantity(uint32_t qty);
        void ChangeQuantity(int32_t qty);
    };

    static ObjectPtr GetObject(uint32_t id);
    static ObjectPtr BuildFromString(const std::string& str);
    std::string ToString() const;
    static Instance CreateInstance(ObjectPtr obj, uint32_t qty);
    
    
    uint32_t GetUID() const;
    ObjectType GetType() const;
    std::string GetName() const;
    uint32_t GetWeight() const;
    uint32_t GetSprite() const;
};

#endif
