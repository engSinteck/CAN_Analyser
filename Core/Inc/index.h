/*
 * index.h
 *
 *  Created on: Jun 17, 2024
 *      Author: rinaldo.santos
 */

#ifndef INC_INDEX_H_
#define INC_INDEX_H_

const unsigned char index_html[] = {
  0x3c, 0x21, 0x44, 0x4f, 0x43, 0x54, 0x59, 0x50, 0x45, 0x20, 0x68, 0x74,
  0x6d, 0x6c, 0x3e, 0x0a, 0x3c, 0x68, 0x65, 0x61, 0x64, 0x3e, 0x0a, 0x09,
  0x3c, 0x74, 0x69, 0x74, 0x6c, 0x65, 0x3e, 0x48, 0x54, 0x4d, 0x4c, 0x20,
  0x61, 0x6e, 0x64, 0x20, 0x43, 0x53, 0x53, 0x20, 0x22, 0x48, 0x65, 0x6c,
  0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x22, 0x3c, 0x2f, 0x74,
  0x69, 0x74, 0x6c, 0x65, 0x3e, 0x0a, 0x09, 0x3c, 0x73, 0x74, 0x79, 0x6c,
  0x65, 0x3e, 0x0a, 0x09, 0x09, 0x62, 0x6f, 0x64, 0x79, 0x20, 0x7b, 0x0a,
  0x09, 0x09, 0x09, 0x62, 0x61, 0x63, 0x6b, 0x67, 0x72, 0x6f, 0x75, 0x6e,
  0x64, 0x2d, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x20, 0x23, 0x32, 0x44,
  0x32, 0x44, 0x32, 0x44, 0x3b, 0x0a, 0x09, 0x09, 0x7d, 0x0a, 0x09, 0x09,
  0x0a, 0x09, 0x09, 0x68, 0x31, 0x20, 0x7b, 0x0a, 0x09, 0x09, 0x09, 0x63,
  0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x20, 0x23, 0x43, 0x32, 0x36, 0x33, 0x35,
  0x36, 0x3b, 0x0a, 0x09, 0x09, 0x09, 0x66, 0x6f, 0x6e, 0x74, 0x2d, 0x73,
  0x69, 0x7a, 0x65, 0x3a, 0x20, 0x33, 0x30, 0x70, 0x78, 0x3b, 0x0a, 0x09,
  0x09, 0x09, 0x66, 0x6f, 0x6e, 0x74, 0x2d, 0x66, 0x61, 0x6d, 0x69, 0x6c,
  0x79, 0x3a, 0x20, 0x4d, 0x65, 0x6e, 0x6c, 0x6f, 0x2c, 0x20, 0x4d, 0x6f,
  0x6e, 0x61, 0x63, 0x6f, 0x2c, 0x20, 0x66, 0x69, 0x78, 0x65, 0x64, 0x2d,
  0x77, 0x69, 0x64, 0x74, 0x68, 0x3b, 0x0a, 0x09, 0x09, 0x7d, 0x0a, 0x09,
  0x09, 0x0a, 0x09, 0x09, 0x70, 0x20, 0x7b, 0x0a, 0x09, 0x09, 0x09, 0x63,
  0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x20, 0x77, 0x68, 0x69, 0x74, 0x65, 0x3b,
  0x0a, 0x09, 0x09, 0x09, 0x66, 0x6f, 0x6e, 0x74, 0x2d, 0x66, 0x61, 0x6d,
  0x69, 0x6c, 0x79, 0x3a, 0x20, 0x22, 0x53, 0x6f, 0x75, 0x72, 0x63, 0x65,
  0x20, 0x43, 0x6f, 0x64, 0x65, 0x20, 0x50, 0x72, 0x6f, 0x22, 0x2c, 0x20,
  0x4d, 0x65, 0x6e, 0x6c, 0x6f, 0x2c, 0x20, 0x4d, 0x6f, 0x6e, 0x61, 0x63,
  0x6f, 0x2c, 0x20, 0x66, 0x69, 0x78, 0x65, 0x64, 0x2d, 0x77, 0x69, 0x64,
  0x74, 0x68, 0x3b, 0x0a, 0x09, 0x09, 0x7d, 0x0a, 0x09, 0x3c, 0x2f, 0x73,
  0x74, 0x79, 0x6c, 0x65, 0x3e, 0x0a, 0x3c, 0x2f, 0x68, 0x65, 0x61, 0x64,
  0x3e, 0x0a, 0x3c, 0x62, 0x6f, 0x64, 0x79, 0x3e, 0x0a, 0x09, 0x3c, 0x68,
  0x31, 0x3e, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c,
  0x64, 0x20, 0x45, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x3c, 0x2f, 0x68,
  0x31, 0x3e, 0x0a, 0x09, 0x3c, 0x70, 0x3e, 0x54, 0x68, 0x69, 0x73, 0x20,
  0x69, 0x73, 0x20, 0x61, 0x20, 0x76, 0x65, 0x72, 0x79, 0x20, 0x62, 0x61,
  0x73, 0x69, 0x63, 0x20, 0x22, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57,
  0x6f, 0x72, 0x6c, 0x64, 0x22, 0x20, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c,
  0x65, 0x20, 0x6d, 0x61, 0x64, 0x65, 0x20, 0x75, 0x70, 0x20, 0x69, 0x6e,
  0x20, 0x48, 0x54, 0x4d, 0x4c, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x43, 0x53,
  0x53, 0x2e, 0x20, 0x53, 0x65, 0x65, 0x20, 0x69, 0x66, 0x20, 0x79, 0x6f,
  0x75, 0x20, 0x63, 0x61, 0x6e, 0x20, 0x63, 0x68, 0x61, 0x6e, 0x67, 0x65,
  0x20, 0x74, 0x68, 0x65, 0x20, 0x73, 0x69, 0x7a, 0x65, 0x20, 0x6f, 0x66,
  0x20, 0x74, 0x68, 0x65, 0x20, 0x68, 0x65, 0x61, 0x64, 0x65, 0x72, 0x20,
  0x74, 0x65, 0x78, 0x74, 0x20, 0x61, 0x62, 0x6f, 0x76, 0x65, 0x2e, 0x3c,
  0x2f, 0x70, 0x3e, 0x0a, 0x3c, 0x2f, 0x62, 0x6f, 0x64, 0x79, 0x3e, 0x0a,
  0x3c, 0x2f, 0x68, 0x74, 0x6d, 0x6c, 0x3e, 0x0a
};
unsigned int index_html_len = 512;


#endif /* INC_INDEX_H_ */
