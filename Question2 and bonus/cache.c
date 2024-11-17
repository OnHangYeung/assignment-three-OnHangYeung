// cache.c

#include "cache.h"

// Initialize Cache
Cache *create_cache() {
    Cache *cache = (Cache *)malloc(sizeof(Cache));
    cache->line_pool = (CacheLinePool *)malloc(sizeof(CacheLinePool));
    cache->time = 0;
    cache->clock_pointer = 0;

    for (int i = 0; i < CACHE_SIZE; i++) {
        //To do: Initialize each line of the Cache
        cache->line_pool->lines[i].address = 0;
        cache->line_pool->lines[i].recent = 0;
        cache->line_pool->lines[i].freq = 0;
        cache->line_pool->lines[i].use_bit = 0;
        cache->line_pool->lines[i].valid = 0;
    }

    return cache;
}


void cache_access_lru(Cache *cache, unsigned int address) {
    //To do: Implement the LRU algorithm
    cache->time++;

     // Check for cache hit
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (cache->line_pool->lines[i].valid && 
            cache->line_pool->lines[i].address == address) {
            // Update last access time for the hit line
            printf("LRU: Cache hit at line %d\n", i);
            cache->line_pool->lines[i].recent = cache->time;
            return;
        }
    }

    // Handling for cache miss  
    int replace_index = 0;
    unsigned int least_recent_time = cache->line_pool->lines[0].recent;

    // Find empty line or least_recent_time line
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (!cache->line_pool->lines[i].valid) {
            replace_index = i;
            break;
        }
        if (cache->line_pool->lines[i].recent < least_recent_time) {
            least_recent_time = cache->line_pool->lines[i].recent;
            replace_index = i;
        }
    }

    // Replace the line
    printf("LRU: Cache miss, replaced line %d\n", replace_index);
    cache->line_pool->lines[replace_index].address = address;
    cache->line_pool->lines[replace_index].recent = cache->time;
    cache->line_pool->lines[replace_index].valid = 1;

}

void cache_access_lfu(Cache *cache, unsigned int address) {
    //To do: Implement the LFU algorithm
     cache->time++;

    // Checking for cache hit
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (cache->line_pool->lines[i].valid && 
            cache->line_pool->lines[i].address == address) {
            // Increment frequency counter
            printf("LFU: Cache hit at line %d\n", i);
            cache->line_pool->lines[i].freq++;
            return;
        }
    }

    // Handling for cache miss 
    int replace_index = 0;
    unsigned int min_freq = cache->line_pool->lines[0].freq;

    // Find empty line or least frequency line
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (!cache->line_pool->lines[i].valid) {
            replace_index = i;
            break;
        }
        if (cache->line_pool->lines[i].freq < min_freq) {
            min_freq = cache->line_pool->lines[i].freq;
            replace_index = i;
        }
    }

    // Replace the line
    printf("LFU: Cache miss, replaced line %d\n", replace_index);
    cache->line_pool->lines[replace_index].address = address;
    cache->line_pool->lines[replace_index].freq = 1;  // Initialize freq to 1
    cache->line_pool->lines[replace_index].valid = 1;
}

void cache_access_clock(Cache *cache, unsigned int address) {
    // Bonus part. You can leave it to blank if you need these bonus.
    // To do: Implement the CLOCK algorithm
    cache->time++;

    // Check for cache hit
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (cache->line_pool->lines[i].valid && 
            cache->line_pool->lines[i].address == address) {
            cache->line_pool->lines[i].use_bit = 1;
            printf("Clock: Cache hit at line %d\n", i);
            return;
        }
    }

    //  Handling cache miss (find empty first)
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (!cache->line_pool->lines[i].valid) {
            cache->line_pool->lines[i].address = address;
            cache->line_pool->lines[i].use_bit = 1;
            cache->line_pool->lines[i].valid = 1;
            printf("Clock: Cache miss, replaced line %d\n", i);
            return;
        }
    }

    // Clock algorithm to find the replaced line
    while (1) {
        if (cache->line_pool->lines[cache->clock_pointer].use_bit == 1) {
            // clear use bit and advance hand
            cache->line_pool->lines[cache->clock_pointer].use_bit = 0;
            cache->clock_pointer = (cache->clock_pointer + 1) % CACHE_SIZE;
        } else {
            // Replace the line
            int replace_index = cache->clock_pointer;
            cache->line_pool->lines[cache->clock_pointer].address = address;
            cache->line_pool->lines[cache->clock_pointer].use_bit = 1;
            cache->clock_pointer = (cache->clock_pointer + 1) % CACHE_SIZE;
            printf("Clock: Cache miss, replaced line %d\n", replace_index);
            break;
        }
    }
}

void free_cache(Cache *cache) {
    free(cache->line_pool);
    free(cache);
}