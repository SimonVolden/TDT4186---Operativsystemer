# TDT4186---Operativsystemer

## Practical 1
 ### TEST CASE 1:
 Set three alarms with different timing, and each a different ringtone,
then see if all alarms goes off in the right order, and with the right sound. 
One should also press "l" and check if all alarms are in the list


### TEST CASE 2:
Set three different alarms with different timings.
One should be written with a illegal time format
this alarm should not be listed with l

### TEST CASE 3:
Set three different alarms with different timings.
Check all alarms are listed with l.
Cancel one of the alarm with c.
Check that the cancelled alarm does not show up in list from l.


### TEST CASE 4:
Set a new alarm, then use exit to quit the program.
Check that the alarm child process is terminated.
The process should be terminated and zombie removed.
