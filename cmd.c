/*******************************************************************
    Copyright (C) 2009 FreakLabs
    All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.
    THIS SOFTWARE IS PROVIDED BY THE THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
    Originally written by Christopher Wang aka Akiba.
    Please post support questions to the FreakLabs forum.
*******************************************************************/
/*!
    \file Cmd.c
    This implements a simple command line interface for the Arduino so that
    its possible to execute individual functions within the sketch. 
*/
/**************************************************************************/
#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "cmd.h"

// text strings for command prompt (stored in flash)
PROGMEM_DECLARE(char const, cmd_prompt[]) = "ITW >> ";
PROGMEM_DECLARE(char const, cmd_unrecog[]) = "ITW: Command not recognized.\n";

/**************************************************************************/
/*!
    Read text string on PROGMEM and write it on USB CDC.
*/
/**************************************************************************/

void chb_cmd_putmsg_PROGMEM(const char *string)
{
	const char PROGMEM_PTR_T ptr = string;
	char val;

	while ((val = PROGMEM_READ_BYTE(ptr))) {
		udi_cdc_putc(val);
		ptr++;
	}
}

/**************************************************************************/
/*!
    Generate the main command prompt
*/
/**************************************************************************/

void chb_cmd_display()
{
   	udi_cdc_putc('\n');
    chb_cmd_putmsg_PROGMEM(cmd_prompt);
}

/**************************************************************************/
/*!
    Parse the command line. This function tokenizes the command input, then
    searches for the command table entry associated with the commmand. Once found,
    it will jump to the corresponding function.
*/
/**************************************************************************/
void chb_cmd_parse(char *cmd)
{
    uint8_t argc, i = 0;
    char *argv[30];
    char buf[50];
    cmd_t *cmd_entry;

    fflush(stdout);

    // parse the command line statement and break it up into space-delimited
    // strings. the array of strings will be saved in the argv array.
    argv[i] = strtok(cmd, " ");
    do
    {
        argv[++i] = strtok(NULL, " ");
    } while ((i < 30) && (argv[i] != NULL));
    
    // save off the number of arguments for the particular command.
    argc = i;

    // parse the command table for valid command. used argv[0] which is the
    // actual command name typed in at the prompt
    for (cmd_entry = cmd_tbl; cmd_entry != NULL; cmd_entry = cmd_entry->next)
    {		
		if (!strcmp(argv[0], cmd_entry->cmd))
        {
            cmd_entry->func(argc, argv);
            chb_cmd_display();
            return;
        }	
	}

    // command not recognized. print message and re-generate prompt.
    strcpy_P(buf, cmd_unrecog);
    
	for (uint8_t i = 0; i < sizeof(buf); i++)
	{
		if (buf[i] != '\0')
		udi_cdc_putc(buf[i]);
		else break;
	}
	
    chb_cmd_display();
}

/**************************************************************************/
/*!
    This function processes the individual characters typed into the command
    prompt. It saves them off into the message buffer unless its a "backspace"
    or "enter" key. 
*/
/**************************************************************************/
void chb_cmd_handler()
{
    char c = udi_cdc_getc();

    switch (c)
    {
    case '\r':
        // terminate the msg and reset the msg ptr. then send
        // it to the handler for processing.
        *msg_ptr = '\0';
		//udi_cdc_putc('\r');
		udi_cdc_putc('\n');
        chb_cmd_parse((char *)msg);
        msg_ptr = msg;
		
        break;
    
    case '\b':
        // backspace
		udi_cdc_putc(c);
        if (msg_ptr > msg)
        {
            msg_ptr--;
        }
        break;
    
    default:
        // check to make sure we're not overflowing the message buffer
        // stay within the msg size - 1 to account for the trailing null
        if ((msg_ptr - msg) < (MAX_MSG_SIZE - 1))
        {
            // normal character entered. add it to the buffer
            udi_cdc_putc(c);
            *msg_ptr++ = c;
        }
        break;
    }
}

/**************************************************************************/
/*!
    This function should be set inside the main loop. It needs to be called
    constantly to check if there is any available input at the command prompt.
*/
/**************************************************************************/
void chb_cmd_poll()
{
    while (udi_cdc_is_rx_ready())
    {
        chb_cmd_handler();
    }
}

/**************************************************************************/
/*!
    Initialize the command line interface. This sets the terminal speed and
    and initializes things. 
*/
/**************************************************************************/
void chb_cmd_init()
{
    // init the msg ptr
    msg_ptr = msg;

    // init the command table
    cmd_tbl_list = NULL;

    //udc_start();
}

/**************************************************************************/
/*!
    Add a command to the command table. The commands should be added in
    at the setup() portion of the sketch. 
*/
/**************************************************************************/
void chb_cmd_add(char *name, void (*func)(int argc, char **argv))
{
    // alloc memory for command struct
    cmd_tbl = (cmd_t *)malloc(sizeof(cmd_t));

    // alloc memory for command name
    char *cmd_name = (char *)malloc(strlen(name)+1);

    // copy command name
    strcpy(cmd_name, name);

    // terminate the command name
    cmd_name[strlen(name)] = '\0';

    // fill out structure
    cmd_tbl->cmd = cmd_name;
    cmd_tbl->func = func;
    cmd_tbl->next = cmd_tbl_list;
    cmd_tbl_list = cmd_tbl;
}