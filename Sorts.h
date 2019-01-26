float getMin(vector<float> list, float lowBound = numeric_limits<float>::min()){
	float min = numeric_limits<float>::max();
	for(int i=0; i<list.size(); i++){
		if(list[i] < min && list[i] > lowBound){
			min = list[i];
		}
	}

	return(min);
}


tuple<int, vector<float>> bruteSort(vector<float> list){
	vector<float> sortedList(list.size());

	sortedList[0] = getMin(list);
	int compares = 2 * list.size();
	for(int i=1; i<sortedList.size(); i++) {
		sortedList[i] = getMin(list, sortedList[i-1]);
		compares += 2 * list.size();
	}

	return(make_tuple(compares, sortedList));
}


tuple<int, vector<float>> bubbleSort(vector<float> list){
	float holder;
	int compares = 0;
	int flips = 1;
	
	while(flips != 0){
		flips = 0;
		for(int i=1; i<list.size(); i++){
			if(list[i-1] > list[i]){
				holder = list[i-1];
				list[i-1] = list[i];
				list[i] = holder;
				flips++;
			}
		}
		compares += list.size() - 1;
	}
	
	return(make_tuple(compares, list));
}