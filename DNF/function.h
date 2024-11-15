#pragma once

class CAlbum;

void ObjectCreate(class CObj* _obj, LayerType _layer);
void ObjectDelete(class CObj* _obj, LayerType _layer);
bool ObjectSort(const CObj* a, const CObj* b);


extern list<CAlbum*> LoadQueue;  // 백그라운드 로딩 큐

void AddLoadQueue(CAlbum* _queue);