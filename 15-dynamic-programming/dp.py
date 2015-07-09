class Solution:
    # @param {string} word1
    # @param {string} word2
    # @return {integer}
    def minDistance(self, word1, word2):
        n = len(word1)
        m = len(word2)
        d = [[100000]*(m+1) for i in range(n+1)]

        if n < m:
            return self.minDistance(word2, word1)
        if 0 == m:
            return n

        d[n][m] = 0
        
        for i in range(n-1, -1, -1):
            d[i][m] = 1 + d[i+1][m]
        for i in range(m-1, -1, -1):
            d[n][i] = 1 + d[n][i+1]

        for i in range(n-1, -1, -1):
            for j in range(m-1, -1, -1):
                d[i][j] = min(d[i][j], 1 + d[i][j+1])
                d[i][j] = min(d[i][j], 1 + d[i+1][j])
                if word1[i] == word2[j]:
                    d[i][j] = min(d[i][j], d[i+1][j+1])
                else :
                    d[i][j] = min(d[i][j], 1 + d[i+1][j+1])
        return d[0][0]

        for l in d:
            for num in l:
                print num
            print '\n'
        
        print d[0][0]


s = Solution()
print s.minDistance('a', 'ab')
