#!/usr/bin/env python
import os
import copy

numbers = [];
def main():
	x = 0
	while x >= 0:
		x = int(raw_input('Enter a number: '))
		if x >= 0:
			numbers.append(x)

	DisplayFirstComeFirstServe(15)
	DisplayShortestSeekTimeFirst(15)
	DisplayElevator(15)


def DisplayFirstComeFirstServe(startingPoint):
	print('\nFirst Come First Served')
	PrintHeaders()
	differenceTotal = 0
	for x in xrange(len(numbers)):
		if x == 0:
			differenceTotal += PrintTableRow(startingPoint, numbers[x])
		else:
			differenceTotal += PrintTableRow(numbers[x-1], numbers[x])

	PrintTotalTraversed(differenceTotal)

def DisplayShortestSeekTimeFirst(startingPoint):
	print('\nShortest Seek Time First')
	PrintHeaders()
	differenceTotal = 0
	currentValue = startingPoint
	numCopy = copy.deepcopy(numbers)
	while len(numCopy) > 0:
		index = IndexToShortestDistance(numCopy, currentValue)
		nextValue = numCopy.pop(index)
		differenceTotal += PrintTableRow(currentValue, nextValue)
		currentValue = nextValue 
	PrintTotalTraversed(differenceTotal)


def DisplayElevator(startingPoint):
	print('\nElevator')
	PrintHeaders()
	differenceTotal = 0
	currentValue = startingPoint
	numCopy = copy.deepcopy(numbers)
	highValue = max(numCopy)
	while (currentValue <= highValue) and (len(numCopy) > 0):
		index = IndexToNextUp(numCopy, currentValue)
		nextValue = numCopy.pop(index)
		differenceTotal += PrintTableRow(currentValue, nextValue)
		currentValue = nextValue
	PrintTotalTraversed(differenceTotal)


def IndexToNextUp(nums, currentValue):
	index = 0
	nextUp = max(nums)
	for x in xrange(len(nums)):
		if (nums[x] >= currentValue) and (nums[x] <= nextUp):
			nextUp = nums[x]
			index = x
	return index


def IndexToShortestDistance(tracks, startingValue):
	index = 0
	smallestDifference = -1
	currentValue = startingValue
	for x in xrange(len(tracks)):
		difference = abs(currentValue - tracks[x])
		if (smallestDifference < 0) or (difference < smallestDifference):
			smallestDifference = difference
			index = x
	return index


def PrintHeaders():
	print('Head Movement\tTracks Traversed')


def PrintTableRow(startPoint, endPoint):
	difference = abs(startPoint - endPoint)
	print('Tracks {0}-{1}\t{2}'.format(startPoint, endPoint, difference)) 
	
	return difference


def PrintTotalTraversed(total):
	print('Total Tracks Traversed\t{0}\n'.format(total))
