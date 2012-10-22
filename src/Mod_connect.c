/****************************************************************
* Jianlong Chai chai.4@wright.edu  June 6th 2012                *
* This function is specified for connect a modbus slave         *
* through RTU. If it succeed, it will return a modbus_t pointer *
* pointing to that slave. If not, then return NULL              *
****************************************************************/
#include  <modbus.h>
#include  <sys/types.h> 
#include  <sys/stat.h> 
#include  <ExternVars.h>
#include  <Defs.h>
#include  <string.h>

extern int login(char *event);
extern void enable(int command);

/*******************************************
* modbus_t * modbusconnection(ctx)			   *
* Used to establish modbus connection      *
*******************************************/
modbus_t * modbusconnection(modbus_t * ctx)
{
	int error,reconnect=1;
	//modbus_t *ctx;
	enable(1);
	ctx = modbus_new_rtu("/dev/ttyS1", 19200, 'N', 8, 1);
	if (ctx == NULL) 
	{
		return NULL;
    }
    modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);

   	modbus_set_slave(ctx, SERVER_ID);

	do{
		//Just open and set the ttyS1 port on client, has nothing to do with the slave board
		error = modbus_connect(ctx);
		reconnect++;
	}while((error == -1)&&(reconnect <=3));
	
	if ( error == 0 ) 
	{ 
		login("Succeed in setting modbus");
	}
	else 	
	{
		login("Failed in setting modbus");
		return NULL;
	}
	return ctx;
}
