#pragma once
#include "CState.h"
#include "CKeyMgr.h"

enum class PlayerSkill
{
    Geardrive,
    Mistheavyrain,
    Setupfire,
    Hazestorm,
    Nebula,
    Scrambleanchor,
    Acrobaticraid,
    Jetstream,
    Mistcroud,
    Setuprage,
    Dizzle,
    Tripleaxel,
    Polaris,
    Sagittarius,

    Basic0,
    Basic1,
    Basic2,
    Exciting,
    Setupattack,

    END
};

vector<Keyboard> AttackKey =
{
    Keyboard::Q,
    Keyboard::W,
    Keyboard::E,
    Keyboard::R,
    Keyboard::T,
    Keyboard::Y,
    Keyboard::A,
    Keyboard::S,
    Keyboard::D,
    Keyboard::F,
    Keyboard::G,
    Keyboard::H,
    Keyboard::NUM_4,
    Keyboard::NUM_5,

    Keyboard::Z,
    Keyboard::LCTRL

};

enum class EffectLayer
{
    a,
    b,
    c,

    END
};

class CAlbumPlayer;
class CArrow;

class CPlayer_Attack :
    public CState
{
    friend class CTimeMgr;
public:
    CPlayer_Attack(wstring _name);
    ~CPlayer_Attack();

private:
    float m_SkillDuration;
    float m_PlaylistTimer;
    float m_IdleTimer;
    float m_Cooltime[(int)PlayerSkill::END];
    PlayerSkill m_CurrentSkill;
    Keyboard m_TappedKey;
    vector<pair<CAlbumPlayer*, float>> m_Playlist;
    vector<pair<vector<CAlbumPlayer*>, float>> m_PlaylistAvt;
    vector<pair<CAlbumPlayer*, float>> m_PlaylistEff[(int)EffectLayer::END];
    int m_PlaylistIdx;
    CAlbumPlayer* m_IdleAnimation;
    vector<CAlbumPlayer*> m_IdleAnimationAvt;

    bool m_Flag[5];
    Vec2D m_Coord;


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();

    void Render();
    void AddPlaylist(CAlbumPlayer* _AP, float _dur) { m_Playlist.push_back(make_pair(_AP, _dur)); }
    void AddPlaylistAvt(vector<CAlbumPlayer*> _APVec, float _dur) { m_PlaylistAvt.push_back(make_pair(_APVec, _dur)); }
    void AddPlaylistEff(CAlbumPlayer* _AP, float _dur, EffectLayer _layer) { m_PlaylistEff[(int)_layer].push_back(make_pair(_AP, _dur)); }
    void ClearPlaylist();

    bool AttackCheck();

private:
    void GeardriveCallback(DWORD_PTR _arrow);
    void MHRArrowCyan(int _angle);
    void MHRArrowCyanCallback0(DWORD_PTR _arrow);
    void MHRArrowCyanCallback1(DWORD_PTR _arrow);

};

