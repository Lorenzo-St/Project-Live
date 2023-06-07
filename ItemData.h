#pragma once
typedef struct invItem invItem;
void loadItems();
void ReleaseItemsStorage(void);

const invItem* getItem(char id);
char chooseId();
#ifdef CPP
std::vector<invItem*> const& getItemList(void);
std::string const* getName(char id);
#endif