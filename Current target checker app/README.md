Problem Statement: 
------------------
A Currency broker company has asked us to build a system that notifies them when a currency pair rate reaches the target rate. User configures the currency pair and target rate.

The URL to get the Rates in XML form: "http://rates.fxcm.com/RatesXML"

Example of the rate: 
<Rate Symbol="EURUSD">
<Bid>1.38022</Bid>
<Ask>1.38042</Ask>
<High>1.38266</High>
<Low>1.37634</Low>
<Direction>0</Direction>
<Last>10:42:43</Last>
</Rate>

Here "Bid" is the current rate. "Ask" is the suggested sell rate. "High" is day high, "Low" is day low, "Direction" is 1=>up, -1=>down, 0=>no change compared to last rate, and "Last" is the time for last tick.

Design considerations:
----------------------
1. You will need a parser to parse the XML. There are several examples online.
2. Start with hard coded user configuration. For example. Pair: EURUSD, Target rate: 1.381 
3. Make it user entry once you have working code.
4. You will periodically check the rates, consider using a schedular.
4. For notification, we will assume that there is a system that will be called to notify users. You only display on the screen if target has been reached.
