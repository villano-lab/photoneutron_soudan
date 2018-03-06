#!/bin/sh

ls -lh logs/ |awk '{arr[$9] = $5}END{for(i in arr){if(match(arr[i],"G")!=0){print arr[i]"\t"i}}}'
ls -lh logs/ |awk '{arr[$9] = $5}END{for(i in arr){if(match(arr[i],"T")!=0){print arr[i]"\t"i}}}'
