#pragma once

class CAlbum;

void ObjectCreate(class CObj* _obj, LayerType _layer);
void ObjectDelete(class CObj* _obj, LayerType _layer);
bool ObjectSort(const CObj* a, const CObj* b);


extern list<CAlbum*> LoadQueue;  // ��׶��� �ε� ť

void AddLoadQueue(CAlbum* _queue);