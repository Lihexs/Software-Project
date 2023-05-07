class Solution(object):
    def search(self, nums, target):
        indx = self.searchPivot(nums, 0, len(nums) - 1)
        if nums[0] <= target <= nums[indx]:
            return self.binarySearch(nums,0,indx,target)
        else:
            return self.binarySearch(nums,indx + 1, len(nums) - 1,target)

    def searchPivot(self, nums, low, high):
        while low < high:
            middle = (low + high) // 2
            if nums[middle] > nums[middle + 1]:
                return middle
            if nums[middle] < nums[high]:
                high = middle
            else:
                low = middle + 1
        return low

    def binarySearch(self, nums, low, high, target):
        while low <= high:
            middle = (low + high) // 2
            if nums[middle] == target:
                return middle
            elif nums[middle] < target:
                low = middle + 1
            else:
                high = middle - 1
        return -1

s = Solution()
print(s.search([8,9,2,3,4], 9))
