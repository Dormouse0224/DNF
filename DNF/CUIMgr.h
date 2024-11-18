#pragma once

class CUI;

class CUIMgr
{
public:
	SINGLETON(CUIMgr);
private:
	CBase* m_DelegateObj;
	DELEGATE_0 m_RenderDelegate0;


public:
	void Tick();

	CUI* GetPriorityUI(CUI* _ParentUI, bool _getSecondary = false, bool _getThird = false);
	CUI* GetSuperParent(CUI* _UI);

	void ReleaseCheck(CUI* _UI);

	void SetRenderDelegate0(CBase* _DelegObj, DELEGATE_0 _DelegFunc) { m_DelegateObj = _DelegObj; m_RenderDelegate0 = _DelegFunc; }
	void ResetRender() { m_DelegateObj = nullptr; m_RenderDelegate0 = nullptr; }
	void Render();
};

