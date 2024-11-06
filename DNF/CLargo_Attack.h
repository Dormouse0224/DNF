#pragma once
#include "CState.h"

enum class LargoSkill
{
    Opening,
    Diagonal_Slash,
    Summon_Clone_Slash,
    Cross_Slash_1,

    END
};

class CAlbumPlayer;
class CPlayer;

class CLargo_Attack :
    public CState
{
public:
    CLargo_Attack();
    ~CLargo_Attack();

private:
    float m_SkillDuration;
    LargoSkill m_CurrentSkill;
    vector<CAlbumPlayer*> m_Playlist;
    vector<CAlbumPlayer*> m_Playlist1;
    int m_PlaylistIdx;
    CAlbumPlayer* m_IdleAnimation;
    CAlbumPlayer* m_IdleAnimation1;

    bool m_IsOpening;
    CPlayer* m_Player;
    bool m_Flag[5];
    Vec2D m_Coord;


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();

    void Render();

    void AddPlaylist(CAlbumPlayer* _AP) { m_Playlist.push_back(_AP); }
    void AddPlaylist1(CAlbumPlayer* _AP) { m_Playlist1.push_back(_AP); }
    void ClearPlaylist();
};

