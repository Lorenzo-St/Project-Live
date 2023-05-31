#pragma once
typedef struct invItem invItem;
void loadItems();
void ReleaseItemsStorage(void);

const invItem* getItem(char id);
char chooseId();
