CurrentBasketID=""
CurrentGoodsID=""
LED=0;
QuantityOfGoods =0
BasketNum =0
tmp="";
typedef struct {
	char basket_id[BASKET_ID_LEN];
	uint8 GoodsNum;
	uint8 offset;
	char prods_id[MAX_PRODS*(OFFSET)];
}Basket;
typedef struct {
	Goods_id[offset-1];
	Quantity;
}Goods;
//FLash
- Quantiy Of Basket
- Basket Length in Bytes
- Bakset 1
 + EmptyFlag
 + struct Basket
- Bakset 2
 + EmptyFlag
 + struct Basket
...

//Indentify a Baskket ID(begine #) or a Goods ID(no begine);
////if (Check Format){
//if Read a Basket ID ->tmp
if(tmp==CurrentBasketID){
  //Close Basket
  CurrentBasketID="";
  //Pack Basket into Flash
  LED = 0;
}else{
  if(CurrentBasketID != ""){
    //Pack Basket into Flash
    LED = 0;
  }
  //Open New Basket
  CurrentBasketID=tmp;
  LED=1;
  }
}

//if Read a Goods ID ->tmp
if(CurrentBasketID != ""){
  if(match(tmp,CurrentGoodsID))
    //Match
    CurrentGoodsID.QuantityOfGoods++;
  else if(&QuantityOfGoods=findinTable(tmp))
    //Exist
    CurrentGoodsID.QuantityOfGoods++;
  else{
    //No Exist and create a new Goods ID
    addNewGoodsToTable(tmp);
    tmp.QuantityOfGoods=1;
  }
}else{
  //warning: please enter a Basket ID
}