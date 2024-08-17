#pragma once

#include <ll/api/event/Event.h>

#include <mc/world/actor/player/Player.h>
#include <mc/world/level/Level.h>
#include <ll/api/event/Cancellable.h>
#include <iostream>

/*#ifdef _WIN32
#   ifdef BUILDING_DLL
#       define LLAPI __declspec(dllexport)
#       define LLNDAPI __declspec(dllexport)
#   else
#       define LLAPI __declspec(dllimport)
#       define LLNDAPI __declspec(dllimport)
#   endif
#else
#   define LLAPI
#   define LLNDAPI
#endif*/

namespace pp {

class RankEvent final : public ll::event::Event
{
    Level& mLevel;
    Player& mPlayer;
public:
    constexpr explicit PPRankExpiredEvent(Level& level,Player& player) : mLevel(level),mPlayer(player) {}

    LLNDAPI Level& level() const;
    LLNDAPI Player& player() const;
};

class PPRankExpiredEvent final : public ll::event::Cancellable<RankEvent>
{
public:
    constexpr explicit PPRankExpiredEvent(Level& level,Player& player) : ll::event::Cancellable<RankEvent>(level, player) {}

    LLAPI void serialize(CompoundTag&) const override;
    LLAPI void deserialize(CompoundTag const&) override;

};
} // namespace ll::event::inline pp
