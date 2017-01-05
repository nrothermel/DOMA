# DOMA
Depth of Market Analyzer. GUI tool for hystorical order book analysis.
When I became curious about how Level 2 financial data can be used by a retail investor I found that there is 
very little information available about the topic. Also, retail investors seem not to be provided with historical Level 2 data by
their brokers. In order to be able to come up trading ideas that make use of Level 2 data I decided to create some tools myself. 
At the current state DOMA can plot a tick chart and display the snapshot of the order book at any time on that chart and provides 
the bid/ask ratio for every snapshot.

This is also my Qt learning project so any kind of contribution is greatly appreciated.

DOMA uses a local MySQL database as datasource and makes heavy use of Emanuel Eichhammer's great QCustomPlot library 
<href>http://www.qcustomplot.com/</href> . In order to set up the database and fill it with data (if you are an Interactive 
Brokers customer) you can use another tool I wrote <href>https://github.com/nrothermel/ibTWS_L2_Miner</href>. In the future 
it will be integrated into DOMA.
