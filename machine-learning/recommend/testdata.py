import csv
import sys
import math
import random
from tqdm import tqdm
import heapq

def readUserMovieRecord():
    record_set = set()
    with open(r"C:\Users\truma\git2\datasets-4-test\MovieLens\tag.csv", encoding='utf-8') as f:
      spamreader = csv.reader(f)
      next(spamreader)
      for row  in spamreader:
          user  = int(row[0])
          movie = int(row[1])
          record_set.add((user, movie))
    return record_set
def tranform2UserView(record_set):
    user_movie_dict = {}
    for row  in record_set:
          user  = row[0]
          movie = row[1]
          if user not in user_movie_dict:
              user_movie_dict[user] = set()
          user_movie_dict[user].add(movie)
    return user_movie_dict
def tranform2MovieView(record_set):
    movie_user_dict = {}
    for row  in record_set:
          user  = row[0]
          movie = row[1]
          if movie not in movie_user_dict:
              movie_user_dict[movie] = set()
          movie_user_dict[movie].add(user)
    return movie_user_dict

