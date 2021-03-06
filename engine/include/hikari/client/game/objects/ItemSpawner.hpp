#ifndef HIKARI_CLIENT_GAME_OBJECTS_ITEMSPAWNER
#define HIKARI_CLIENT_GAME_OBJECTS_ITEMSPAWNER

#include "hikari/client/game/objects/Spawner.hpp"

#include <memory>
#include <string>

namespace hikari {

    class CollectableItem;
    
    class ItemSpawner : public Spawner {
    private:
        int spawnedItemId;
        std::string itemName;
        bool canSpawnAgain;

        void handleEntityDeathEvent(EventDataPtr event);

    public:
        ItemSpawner(const std::string & itemNam);
        virtual ~ItemSpawner();

        virtual void performAction(GameWorld & world);
        virtual void attachEventListeners(EventBus & EventBus);
        virtual void detachEventListeners(EventBus & EventBus);

        virtual bool canSpawn() const;

        virtual void onSleep();

        //
        // GameObject overrides
        //
        virtual void onActivated();
        virtual void onDeactivated();
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ITEMSPAWNER