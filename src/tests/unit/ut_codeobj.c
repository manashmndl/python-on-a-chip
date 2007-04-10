/*
 * PyMite - A flyweight Python interpreter for 8-bit microcontrollers and more.
 * Copyright 2007 Dean Hall
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


/**
 * Code Object Unit Tests
 *
 * Tests the Code Object implementation.
 *
 * Log
 * ---
 *
 * 2007/03/10   First.
 */


#include "CuTest.h"
#include "pm.h"


/* BEGIN unit tests ported from Snarf */
/**
 * The following source code was compiled to an image using pmImgCreator.py
 * from svn revision [195] using this command line:
 *
 *      ../../tools/pmImgCreator.py  -c -s -o utco.img.c utco.py
 * 

def main():

    s = 0
    A = (0,1,2,3)
    B = (3,2,1,0)

    for i in A:
        s += i

    for i in B:
        s -= i

    for i in A:
        for j in B:
            s += i*j

    return s

main()

 */
uint8_t test_code_image0[] =
{
    0x0A, 0xFB, 0x00, 0x00, 0x01, 0x00, 0x04, 0x02, 
    0x03, 0x04, 0x00, 0x6D, 0x61, 0x69, 0x6E, 0x03, 
    0x04, 0x00, 0x75, 0x74, 0x63, 0x6F, 0x04, 0x02, 
    0x0A, 0xCE, 0x00, 0x00, 0x09, 0x05, 0x04, 0x01, 
    0x03, 0x04, 0x00, 0x6D, 0x61, 0x69, 0x6E, 0x04, 
    0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0x01, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 
    0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x04, 0x04, 
    0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 
    0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x01, 
    0x03, 0x00, 0x00, 0x00, 0x04, 0x04, 0x01, 0x03, 
    0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 
    0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
    0x00, 0x00, 0x64, 0x01, 0x00, 0x7D, 0x00, 0x00, 
    0x64, 0x05, 0x00, 0x7D, 0x01, 0x00, 0x64, 0x06, 
    0x00, 0x7D, 0x02, 0x00, 0x78, 0x18, 0x00, 0x7C, 
    0x01, 0x00, 0x44, 0x5D, 0x10, 0x00, 0x7D, 0x03, 
    0x00, 0x7C, 0x00, 0x00, 0x7C, 0x03, 0x00, 0x37, 
    0x7D, 0x00, 0x00, 0x71, 0x19, 0x00, 0x57, 0x78, 
    0x18, 0x00, 0x7C, 0x02, 0x00, 0x44, 0x5D, 0x10, 
    0x00, 0x7D, 0x03, 0x00, 0x7C, 0x00, 0x00, 0x7C, 
    0x03, 0x00, 0x38, 0x7D, 0x00, 0x00, 0x71, 0x34, 
    0x00, 0x57, 0x78, 0x2D, 0x00, 0x7C, 0x01, 0x00, 
    0x44, 0x5D, 0x25, 0x00, 0x7D, 0x03, 0x00, 0x78, 
    0x1C, 0x00, 0x7C, 0x02, 0x00, 0x44, 0x5D, 0x14, 
    0x00, 0x7D, 0x04, 0x00, 0x7C, 0x00, 0x00, 0x7C, 
    0x03, 0x00, 0x7C, 0x04, 0x00, 0x14, 0x37, 0x7D, 
    0x00, 0x00, 0x71, 0x5C, 0x00, 0x57, 0x71, 0x4F, 
    0x00, 0x57, 0x7C, 0x00, 0x00, 0x53, 0x00, 0x64, 
    0x00, 0x00, 0x84, 0x00, 0x00, 0x5A, 0x00, 0x00, 
    0x65, 0x00, 0x00, 0x83, 0x00, 0x00, 0x01, 0x64, 
    0x01, 0x00, 0x53, 
};


/**
 * Tests co_loadFromImg():
 *      retval is OK
 *      pimg points to one byte past the end of the image after image is loaded
 */
void
ut_co_loadFromImg_000(CuTest *tc)
{
    PmReturn_t retval;
    uint8_t *paddr = test_code_image0;
    uint8_t *pimg = test_code_image0;
    pPmObj_t pcodeobject;
    uint16_t size;
    
    pm_init(MEMSPACE_RAM, C_NULL);

    /* Check the return value of the load function */
    retval = obj_loadFromImg(MEMSPACE_PROG, &pimg, &pcodeobject);
    CuAssertTrue(tc, retval == PM_RET_OK);
    
    /* Size occurs one byte after the type */
    paddr++;
    size = mem_getWord(MEMSPACE_PROG, &paddr);
    
    /* Check that pimg now points to one byte past the end of the image */
    CuAssertTrue(tc, pimg == (test_code_image0 + size));
}
/* END unit tests ported from Snarf */


/** Make a suite from all tests in this file */
CuSuite *getSuite_testCodeObj(void)
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, ut_co_loadFromImg_000);

    return suite;
}
