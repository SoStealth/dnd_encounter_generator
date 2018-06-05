#include "entities.hpp"

int main(int argc, char* argv[]) {
	init_random();
	char* myentity = "Bruno Ivan,1,12,20,10,14,8,6,6,0,0,0,2";
	Barbarian entity(myentity);
	printf("Hai incontrato %s\n",entity.get_name());
	char* myitem = "Pozione annacquata di Levin,8,1";
	entity.equip_item(myitem);
	for(int i=0;i<3;i++) {
		printf("Vediamo cosa %s decide di fare: %d\n",entity.get_name(),entity.act());
		printf("Colpiamolo un po'\n");
		entity.hit(4);
	}
	printf("Finito.\n");
	char* myentity1 = "Lino Mendol,1,12,8,12,10,14,14,16,0,0,0,2";
	Bard entity1(myentity1);
	printf("Hai incontrato %s\n",entity1.get_name());
	char* myitem1 = "Pozione annacquata di Levin,8,1";
	entity1.equip_item(myitem1);
	for(int i=0;i<3;i++) {
		printf("Vediamo cosa %s decide di fare: %d\n",entity1.get_name(),entity1.act());
		printf("Colpiamolo un po'\n");
		entity1.hit(4);
	}
	char* myentity2 = "Padre Pio,1,12,8,12,10,14,14,16,0,0,0,2";
	Cleric entity2(myentity2);
	printf("Hai incontrato %s\n",entity2.get_name());
	char* myitem2 = "Pozione annacquata di Levin,8,1";
	entity2.equip_item(myitem2);
	for(int i=0;i<3;i++) {
		printf("Vediamo cosa %s decide di fare: %d\n",entity2.get_name(),entity2.act());
		printf("Colpiamolo un po'\n");
		entity2.hit(4);
	}
	char* myentity3 = "Panoramix,1,12,6,8,10,16,16,12,0,0,0,2";
	Druid entity3(myentity3);
	printf("Hai incontrato %s\n",entity3.get_name());
	char* myitem3 = "Pozione annacquata di Levin,8,1";
	entity3.equip_item(myitem3);
	for(int i=0;i<3;i++) {
		printf("Vediamo cosa %s decide di fare: %d\n",entity3.get_name(),entity3.act());
		printf("Colpiamolo un po'\n");
		entity3.hit(4);
	}
	char* myentity4 = "Merlone,1,12,14,10,8,14,6,16,0,0,0,2";
	Sorcerer entity4(myentity4);
	printf("Hai incontrato %s\n",entity4.get_name());
	char* myitem4 = "Pozione annacquata di Levin,8,1";
	entity4.equip_item(myitem4);
	for(int i=0;i<3;i++) {
		printf("Vediamo cosa %s decide di fare: %d\n",entity4.get_name(),entity4.act());
		printf("Colpiamolo un po'\n");
		entity4.hit(4);
	}
	char* myentity5 = "Enrico Vasaio,1,12,6,14,8,16,14,10,0,0,0,2";
	Wizard entity5(myentity5);
	printf("Hai incontrato %s\n",entity5.get_name());
	char* myitem5 = "Pozione annacquata di Levin,8,1";
	entity5.equip_item(myitem5);
	for(int i=0;i<3;i++) {
		printf("Vediamo cosa %s decide di fare: %d\n",entity5.get_name(),entity5.act());
		printf("Colpiamolo un po'\n");
		entity5.hit(4);
	}
	char* myentity6 = "Kung Fury,1,12,16,14,10,6,14,8,0,0,0,2";
	Monk entity6(myentity6);
	printf("Hai incontrato %s\n",entity6.get_name());
	char* myitem6 = "Pozione annacquata di Levin,8,1";
	entity6.equip_item(myitem6);
	for(int i=0;i<3;i++) {
		printf("Vediamo cosa %s decide di fare: %d\n",entity6.get_name(),entity6.act());
		printf("Colpiamolo un po'\n");
		entity6.hit(4);
	}
	char* myentity7 = "Sir Iano,1,12,16,14,14,6,10,8,0,0,0,2";
	Fighter entity7(myentity7);
	printf("Hai incontrato %s\n",entity7.get_name());
	char* myitem7 = "Pozione annacquata di Levin,8,1";
	entity7.equip_item(myitem7);
	for(int i=0;i<3;i++) {
		printf("Vediamo cosa %s decide di fare: %d\n",entity7.get_name(),entity7.act());
		printf("Colpiamolo un po'\n");
		entity7.hit(4);
	}
	char* myentity8 = "Yoghi Bubu,1,12,14,14,10,8,16,6,0,0,0,2";
	Ranger entity8(myentity8);
	printf("Hai incontrato %s\n",entity8.get_name());
	char* myitem8 = "Pozione annacquata di Levin,8,1";
	entity8.equip_item(myitem8);
	for(int i=0;i<3;i++) {
		printf("Vediamo cosa %s decide di fare: %d\n",entity8.get_name(),entity8.act());
		printf("Colpiamolo un po'\n");
		entity8.hit(4);
	}
	char* myentity9 = "Lu Pin,1,12,6,16,8,14,14,10,0,0,0,2";
	Rogue entity9(myentity9);
	printf("Hai incontrato %s\n",entity9.get_name());
	char* myitem9 = "Pozione annacquata di Levin,8,1";
	entity9.equip_item(myitem9);
	for(int i=0;i<3;i++) {
		printf("Vediamo cosa %s decide di fare: %d\n",entity9.get_name(),entity9.act());
		printf("Colpiamolo un po'\n");
		entity9.hit(4);
	}
	char* myentity0 = "Hogardo Gagliardo,1,12,14,8,14,6,10,16,0,0,0,2";
	Paladin entity0(myentity0);
	printf("Hai incontrato %s\n",entity0.get_name());
	char* myitem0 = "Pozione annacquata di Levin,8,1";
	entity0.equip_item(myitem0);
	for(int i=0;i<3;i++) {
		printf("Vediamo cosa %s decide di fare: %d\n",entity0.get_name(),entity0.act());
		printf("Colpiamolo un po'\n");
		entity0.hit(4);
	}
	char* myentityo = "un cubo di gelatina,1,12,1,1,1,1,1,1,0,0,0,2,20";
	Monster entityo(myentityo);
	printf("Hai incontrato %s\n",entityo.get_name());
	char* myitemo = "Pozione annacquata di Levin,8,1";
	entityo.equip_item(myitemo);
	for(int i=0;i<3;i++) {
		printf("Vediamo cosa %s decide di fare: %d\n",entityo.get_name(),entityo.act());
		printf("Colpiamolo un po'\n");
		entityo.hit(4);
	}
	return 0;
}