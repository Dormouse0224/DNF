#pragma once

class CAlbum;

void ObjectCreate(class CObj* _obj, LayerType _layer);
void ObjectDelete(class CObj* _obj, LayerType _layer);
bool ObjectSort(const CObj* a, const CObj* b);


extern list<CAlbum*> LoadQueue;  // 백그라운드 로딩 큐
extern list<wstring> ReadQueue;  // 백그라운드 파일리딩 큐
extern std::mutex queueMutex;
extern std::mutex queueMutex1;
extern std::condition_variable loadCV;

void AddLoadQueue(CAlbum* _queue);
void AddReadQueue(wstring _queue);
