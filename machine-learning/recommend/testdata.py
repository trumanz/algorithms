import csv
import sys
import math
import random
from tqdm import tqdm
import heapq

def readUserMovieRecord():
    record_set = set()
    x = []
    with open(r"C:\Users\truma\git2\datasets-4-test\MovieLens\tag.csv", encoding='utf-8') as f:
      spamreader = csv.reader(f)
      next(spamreader)
      for row  in spamreader:
          if len(row) != 4 or row[3].find('2014') < 0:
              continue
          user  = int(row[0])
          movie = int(row[1])
          #if user >= 18:
          #    continue
          #if movie not in [49530]:
          #    continue
          record_set.add((user, movie))
          x.append((user,movie))
    return x
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

