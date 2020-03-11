/******************************************************************************
* File			: ChatLog.h
* Author		: Hong SungBock
* Copyright		: (��)NTL
* Date			: 2008. 7. 2
* Abstract		: 
*****************************************************************************
* Desc			: ä�� �α׸� ���
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "ChatDefine.h"

struct sChatLog
{
	std::wstring		wstrString;
	RwUInt8				byChatType;
};

typedef std::list<sChatLog*>					LIST_CHATLOG;
typedef std::list<sChatLog*>::iterator			ITER_CHATLOG;


class CChatReceiver : public RWS::CEventHandler
{
public:
	CChatReceiver();
	virtual ~CChatReceiver();

	RwBool			Create();
	VOID			Destroy();

	VOID			RegisterNotify(CChatDisplayGui* pDisplayDialog);
	VOID			UnregisterNotify(CChatDisplayGui* pDisplayDialog);

	ITER_CHATLOG	Begin();
	ITER_CHATLOG	End();

protected:
	virtual VOID	HandleEvents(RWS::CMsg &msg);

	VOID			Chatlog_Save(RwUInt8 byType, const WCHAR* pText);
	VOID			Chatlog_Notify(RwUInt8 byType, const WCHAR* pwcText);
	WCHAR*			GetChatName(RwUInt8 byType);

protected:
	LIST_CHATLOG		m_listChatLog;			///< ä�� �α�
	LIST_NOTIFY			m_listNotify;			///< ä�� ������ ���ŵǰų� ���� ���� �˸�
};