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

MPMC_LFQ(remove dummy ver)

-		popRecs	0x000001c3b6e30010 {min=14 max=882196 total=857482668 ...}	st_recode *
		min	14	__int64
		max	882196	__int64
		total	857482668	__int64
		count	10000000	__int64
-		pushRecs	0x000001c3b6d593f0 {min=14 max=862043 total=866786207 ...}	st_recode *
		min	14	__int64
		max	862043	__int64
		total	866786207	__int64
		count	10000000	__int64
		
-		popRecs	0x000002434f869440 {min=14 max=844886 total=874401822 ...}	st_recode *
		min	14	__int64
		max	844886	__int64
		total	874401822	__int64
		count	10000000	__int64
-		pushRecs	0x000002434f869080 {min=14 max=902258 total=863727235 ...}	st_recode *
		min	14	__int64
		max	902258	__int64
		total	863727235	__int64
		count	10000000	__int64
		
-		popRecs	0x0000029bfc38e9d0 {min=14 max=871605 total=895649888 ...}	st_recode *
		min	14	__int64
		max	871605	__int64
		total	895649888	__int64
		count	10000000	__int64
-		pushRecs	0x0000029bfc389c80 {min=14 max=931112 total=851281634 ...}	st_recode *
		min	14	__int64
		max	931112	__int64
		total	851281634	__int64
		count	10000000	__int64

-		popRecs	0x0000026adf594eb0 {min=14 max=973980 total=866379871 ...}	st_recode *
		min	14	__int64
		max	973980	__int64
		total	866379871	__int64
		count	10000000	__int64
-		pushRecs	0x0000026adf599c30 {min=14 max=895027 total=878260264 ...}	st_recode *
		min	14	__int64
		max	895027	__int64
		total	878260264	__int64
		count	10000000	__int64

나열된 일부 데이터들처럼 86xxxxxxx ~ 89xxxxxxx 사이에서 측정됨.

garbageCare 에다 미리 메모리를 할당하든 그렇지 않았든 별 차이는 없었음.


MPMC_LFQ(old ver)

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
아직은 boost 가 편하고 좋다.


