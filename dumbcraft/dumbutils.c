#include "dumbutils.h"
#include "dumbcraft.h"
#include <util10.h>
#include <string.h>

void UpdateSlot( uint8_t window, uint8_t slot, uint8_t count, uint16_t id, uint8_t damage )
{
	StartSend();
	Sbyte( 0x16 );  //Updated (But rest of packet might be wrong)
	Sbyte( window ); 
	Sshort( slot );
	Sshort( id );
	Sbyte( count );
	Sbyte( damage );
	Sbyte( 0x00 ); //??????
	Sbyte( 0xff );
	Sbyte( 0xff ); 
	DoneSend();
}

void GPChat( const char * text )
{
	uint16_t chatlen = strlen( text );
	StartSend();
	Sbyte( 0x0F ); //UPDATED
	Svarint( chatlen + 11 );
	Sbuffer( (const uint8_t*)"{\"text\":\"", 9 );
	Sbuffer( (const uint8_t*)text, chatlen );
	Sbyte( '"' );
	Sbyte( '}' );
	DoneSend();
}

void SpawnEntity( uint16_t eid, uint16_t uuid, uint8_t type, uint16_t x, uint16_t y, uint16_t z )
{
	StartSend();
	Sbyte( 0x28 );  //[UPDATED]
	Sint( eid );
	DoneSend();

	StartSend();
	Sbyte( 0x00 ); //Updated
	Suuid( uuid );
	Sbyte( type );
	Sdouble( x );
	Sdouble( y );
	Sdouble( z );
	Sbyte( 0 );
	Sbyte( 0 );
	Sint( type );
	Sshort( 0 );
	Sshort( 0 );
	Sshort( 0 );
/*
	Sbyte( 0x00 ); //Byte, Index 0
	Sbyte( 0x00 ); //no parameters.
	Sbyte( 127 );*/
	DoneSend();
}

void EntityUpdatePos( uint16_t entity, uint16_t x, uint16_t y, uint16_t z, uint8_t yaw, uint8_t pitch )
{
	StartSend();
	Sbyte( 0x26 ); //UPDATED, MAYBE!
	Svarint( entity );
	Sshort( x );
	Sshort( y );
	Sshort( z );
	Sfloat( yaw );
	Sfloat( pitch );
	Svarint( 0 ); //no teleport
	DoneSend();
}

//Update a sign at a specific location with a string and a numerical value.
void SignUp( uint8_t x, uint8_t y, uint8_t z, const char* st, uint8_t val )
{
	char stmp1[5];
	char stmp2[5];

	Uint8To10Str( stmp1, val );
	Uint8To16Str( stmp2+2, val );
	stmp2[0] = '0'; stmp2[1] = 'x';
	SignTextUp( x, y, z, stmp1, stmp2 );
}

void InternalSendPosition (uint8_t x, uint8_t y, uint8_t z )
{
	//uint32_t slp = ((uint32_t)x & 0x3FFFFFF) << 38 | ((uint32_t)y & 0xFFF) << 26 | ((uint32_t)z & 0x3FFFFFF);
	Sbyte( 0x00 ); //bits 56...
	Sbyte( 0x00 ); //bits 48... (Would be x>>10...) but we're limited in size.
	Sbyte( (x>>2) ); //bits 40...
	Sbyte( ((x & 0x03)<<6) | ((y>>6)&0x3f) ); //bits 32...  XXX Not sure if Y is right.
	Sbyte( (y & 0x3f)<<2 ); //bits 24...
	Sbyte( 0x00 ); //bits 16...
	Sbyte( 0x00 ); //bits 8 ...
	Sbyte( z ); //bits 0 ...
}

void SignTextUp( uint8_t x, uint8_t y, uint8_t z, const char * line1, const char * line2 )
{
	//Sign updates do not work. XXX TODO.
/*
	StartSend();
	Sbyte( 0x09 ); //[UPDATED]  (Update entity)
	InternalSendPosition( x, y, z );
	Sbyte( 9 ); // "Set text on sign"
	Sstring( line1, -1 );
	Sstring( " ", 1 );
	Sstring( line2, -1 );
	Sstring( " ", 1 );
	DoneSend();
*/
}


//Update a block at a given x, y, z (good for 0..255 in each dimension)
void SblockInternal( uint8_t x, uint8_t y, uint8_t z, uint8_t bt, uint8_t meta )
{
	uint16_t tblockmeta = (bt<<4) | meta;

	StartSend();
	Sbyte(0x0b);  //[UPDATED]
	InternalSendPosition( x, y, z );
	Svarint( tblockmeta ); //block type
	DoneSend();

}
