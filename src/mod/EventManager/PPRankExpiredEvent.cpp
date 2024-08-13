#include "PPRankExpiredEvent.h"
#include <ll/api/event/Emitter.h>

#include <mc/nbt/CompoundTag.h>
#include <iostream>
#include <ll/api/Logger.h>

namespace ll::event::inline pp {

void PPRankExpiredEvent::serialize(CompoundTag& nbt) const {
    Event::serialize(nbt);
    nbt["world"] = (uintptr_t)&level();
    nbt["player"] = (uintptr_t)&player();
}

Level& PPRankExpiredEvent::level() const {
    return mLevel;
}

Player& PPRankExpiredEvent::player() const {
    return mPlayer;
}

// Переименованная и исправленная функция. Переход от статической функции к правильному определению
std::unique_ptr<EmitterBase> emitterFactory(ListenerBase& listener) {
    return std::make_unique<PPRankExpiredEventEmitter>(listener);
}

class PPRankExpiredEventEmitter : public Emitter<emitterFactory, PPRankExpiredEvent> {
public:
    PPRankExpiredEventEmitter(ListenerBase& listener) : Emitter(listener) {
        ll::Logger logg("PurePerms");
        logg.info("Emit!");
    }
};

// Пример корректного использования
std::unique_ptr<EmitterBase> createEmitter(ListenerBase& listener) {
    return std::make_unique<PPRankExpiredEventEmitter>(listener);
}

/*static std::unique_ptr<EmitterBase> emitterFactory(ListenerBase&) {
    return std::make_unique<PPRankExpiredEventEmitter>();
}*/
}
/*LL_TYPE_INSTANCE_HOOK(
    ServerStoppingEventHook,
    HookPriority::Normal,
    ServerInstance,
    &ServerInstance::leaveGameSync,
    void
) {
    EventBus::getInstance().publish(ServerStoppingEvent(*this));
    origin();
}

static std::unique_ptr<EmitterBase> emitterFactory(ListenerBase&);
class ServerStoppingEventEmitter : public Emitter<emitterFactory, ServerStoppingEvent> {
    memory::HookRegistrar<ServerStoppingEventHook> hook;
};

static std::unique_ptr<EmitterBase> emitterFactory(ListenerBase&) {
    return std::make_unique<ServerStoppingEventEmitter>();
}*/
