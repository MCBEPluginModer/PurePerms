#pragma once

#include <ll/api/event/Event.h>

#include <mc/world/actor/player/Player.h>
#include <mc/world/level/Level.h>
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

class RankEvent final : public Event
{
    Level& mLevel;
    Player& mPlayer;
public:
    constexpr explicit PPRankExpiredEvent(Level& level,Player& player) : mLevel(level),mPlayer(player) {}

    LLAPI void serialize(CompoundTag&) const override;

    LLNDAPI Level& level() const;
    LLNDAPI Player& player() const;
};

class PPRankExpiredEvent final : public ll::event::Cancellable<RankEvent>
{
    Level& mLevel;
    Player& mPlayer;
public:
    constexpr explicit PPRankExpiredEvent(Level& level,Player& player) : mLevel(level),mPlayer(player),Cancellable(level,player) {}

    LLAPI void serialize(CompoundTag&) const override;
    LLAPI void deserialize(CompoundTag const&) override;

    LLNDAPI Level& level() const;
    LLNDAPI Player& player() const;
};
} // namespace ll::event::inline pp
