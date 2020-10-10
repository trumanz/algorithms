import csv
import sys
import math
import random
from tqdm import tqdm
import heapq
class MoviePref():
    def  __init__(self, movie, perf):
        self.movie = movie
        self.perf = perf
    def __lt__(self, oth) :
        return self.perf < oth.perf

class UserCF:
    def train(self, train_movie_user_dict):
        self.train_movie_user_dict = train_movie_user_dict
        print("calculating user similarity")
        self.weight_of_user_similarity = self.__cal_user_similarity(train_movie_user_dict)

       
    #calculator user similarity
    def __cal_user_similarity(self, movie_user_dict):
      from collections import defaultdict
      count_same_movie = defaultdict(defaultdict)  #C[i][j] represent same movie count of user i and j
      weight_of_similarity = defaultdict(defaultdict)
      user_movie_count = defaultdict()
      for movie,users in tqdm( movie_user_dict.items() ):
        for i in users:
            if i not in  user_movie_count:
                user_movie_count[i] = 0.0
            user_movie_count[i]+= 1.0
            for j in users:
                if i == j:
                    continue
                if i not in count_same_movie[i].keys():
                    count_same_movie[i][j] = 0.0
                #C[i][j]+=1
                count_same_movie[i][j] += 1.0/( math.log( 1.0 + len(users)))  #punish hot movie, 
      for i, jcount in count_same_movie.items():
        for j,count in jcount.items():
            denominator =  math.sqrt ( user_movie_count[i] * user_movie_count[j] )
            weight_of_similarity[i][j] = count / denominator
      return weight_of_similarity

    def recommend(self, user, k, n):
      first_n_movies = []
      if user not in self.weight_of_user_similarity:
          return first_n_movies
      us = self.weight_of_user_similarity[user]
      first_k_similar_user = [(u,w) for u, w in sorted(us.items(), key=lambda kv : kv[1], reverse=False)]
      first_k_similar_user = first_k_similar_user[0:k]

      for movie in self.train_movie_user_dict:
        pref = 0.0
        users_of_movie = self.train_movie_user_dict[movie]
        if  user in users_of_movie:
             continue
        for  user, weight_of_similary in first_k_similar_user:
             if user in users_of_movie:
                 pref += weight_of_similary * 1.0
        mr = MoviePref(movie, pref)
        heapq.heappush(first_n_movies, mr)
        if len(first_n_movies) > n:
            heapq.heappop(first_n_movies)
      x = [ heapq.heappop(first_n_movies).movie for i in range(len(first_n_movies)) ]
      x.reverse()
      return x
