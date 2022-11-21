#pragma once
int moveBullets(bullet* bullets, enemy* en, enemy* bosses, player* pl, item* items, building buildings[]);
int moveBosses(enemy* bosses, building* buildings);
int moveEnemies(enemy* en, building* buildings);