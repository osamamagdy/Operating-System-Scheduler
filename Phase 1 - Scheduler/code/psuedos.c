void SRT(){ // Or Priority Based
    Heap PQ();
    int currRemaining = INT_MAX;
    //int currPriority = INT_MAX;
    while(!empty(PQ) && !empty(array))
    {
        if(ArrivingProcess arrived)
        {
            ArrivingProcess.priroity = ArrivingProcess.runtime; //
            if(currRemaining > ArrivingProcess.priroity) // if(currPriority > ArrivingProcess.priroity)
            {
                stop(CurrProcess);
                CurrProcess.priority = currRemaining; //
                PQ.push(CurrProcess); 
                currRemaining = ArrivingProcess.priroity; //currPriority = ArrivingProcess.priroity
                CurrProcess = ArrivingProcess;
                start(CurrProcess);
            }
            else
            {
                PQ.push(ArrivingProcess);
            }
        }
        if(currRemaining) 
            currRemaining--;
        else
        {
            stop(CurrProcess);
            CurrProcess = PQ.pop();
            currRemaining = CurrProcess.priority; //currPriority = CurrProcess.priority
            start(CurrProcess);
        }
    }
}

Calculations:

at start:           WeightedTurnaround = 0;
                    Count = 0;
                    CPURunTime = 0;
                    WaitingTime = 0;

at each arrival:    CPURunTime += process.runtime;
                    WaitingTime += (getClk() - ArrivingProcess.arrivaltime);
                    Count++;

at each finish:     Turnaround = (getClk() - FinishingProcess.arrivaltime)
                    WeightedTurnaround += Turnaround / (float) FinishingProcess.runtime;

at finish:          TotalCPUTime = getClk();                                        
                    CPUUtil = CPURunTime / (float)TotalCPUTime;
                    AvgWaitingTime = WaitingTime / (float)Count;
                    AvgWeightedTurnaround = WeightedTurnaround / (float)Count;



