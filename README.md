# MPMC_Queue

다수의 쓰레드가 push, pop 을 할 수 있는 queue.

테스트 환경: 
cpu: amd Phenom(tm)2 X4 960T processor 
ram: 8GB

테스트 데이터: 
push 시도할때 얼마나 빨리 완료되는가, 빼낼 데이터가 있을때, pop 시도가 얼마나 빨리 완료되는가.

빼낼 데이터가 없을때의 pop 은 데이터로 취급하지 않기 때문에 
편의를 위해 queue 가 비워지지 않도록 push, pop 을 조절.


MPMC_LFQueue:

MPMC_LFQ

-		popRecs	0x000001d7268d8020 {min=14 max=1460592 total=884103339 ...}	st_recode *
		min	14	__int64
		max	1460592	__int64
		total	884103339	__int64
		count	10000000	__int64
-		pushRecs	0x000001d7266c93e0 {min=14 max=873516 total=893300611 ...}	st_recode *
		min	14	__int64
		max	873516	__int64
		total	893300611	__int64
		count	10000000	__int64

-		popRecs	0x000002b31311d6b0 {min=14 max=1547210 total=889457009 ...}	st_recode *
		min	14	__int64
		max	1547210	__int64
		total	889457009	__int64
		count	10000000	__int64
-		pushRecs	0x000002b313119b80 {min=14 max=884576 total=879607113 ...}	st_recode *
		min	14	__int64
		max	884576	__int64
		total	879607113	__int64
		count	10000000	__int64

나열된 일부 데이터들처럼 87xxxxxxx ~ 91xxxxxxx 사이에서 측정.



boost queue

-		pushRecs[0]	{min=14 max=1343706 total=875277788 ...}	st_recode
		min	14	__int64
		max	1343706	__int64
		total	875277788	__int64
		count	10000000	__int64
-		popRecs[0]	{min=7 max=2238103 total=992171747 ...}	st_recode
		min	7	__int64
		max	2238103	__int64
		total	992171747	__int64
		count	10000000	__int64

-		pushRecs[0]	{min=14 max=880222 total=897663200 ...}	st_recode
		min	14	__int64
		max	880222	__int64
		total	897663200	__int64
		count	10000000	__int64
-		popRecs[0]	{min=14 max=1546783 total=911207808 ...}	st_recode
		min	14	__int64
		max	1546783	__int64
		total	911207808	__int64
		count	10000000	__int64
    
나열된 일부 데이터들처럼 MPMC_LFQ 와 거의 흡사.
pool 의 공간확보를 미리 하는 기능은 사용해보지 않았다.
    



