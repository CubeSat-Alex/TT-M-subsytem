#define ldr1  A0
#define ldr2  A1
#define ldr3  A2
#define ldr4  A3


void LDR_setup()
{
    pinMode(ldr1, INPUT);
    pinMode(ldr2, INPUT);
    pinMode(ldr3, INPUT);
    pinMode(ldr4, INPUT);
}
int get_ldr1()
{ 
    return analogRead(ldr1);
}
int get_ldr2()
{
    return analogRead(ldr2);
}
int get_ldr3()
{
    return analogRead(ldr3);
}
int get_ldr4()
{
    return analogRead(ldr4);
}
