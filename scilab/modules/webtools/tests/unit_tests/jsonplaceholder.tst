// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI-Group
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// This test use JSONPlaceholder as a simple fake REST API for testing and prototyping.
// https://github.com/typicode/jsonplaceholder

root = "http://jsonplaceholder.typicode.com";

// HTTP GET
exp_result.userId = 1;
exp_result.id = 1;
exp_result.title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit";
exp_result.body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto";
assert_checkequal(http_get(root + "/posts/1"), exp_result);

res = http_get(root + "/posts");
assert_checkequal(size(res), [1, 100]);
assert_checkequal(res(50).userId, 5);
assert_checkequal(res(50).id, 50);

res = http_get(root + "/posts?userId=1");
assert_checkequal(size(res), [1 10]);
assert_checkequal(res(1), exp_result);

// HTTP POST
clear exp_result;
data.userId = 12;
data.title  = "A title";
data.body   = "A body";
exp_result  = data;
exp_result.id = 101;
assert_checkequal(http_post(root+"/posts", data), exp_result);

// HTTP PUT
clear data;
data.userId = 12;
data.title  = "A title";
data.body   = "A body";
data.id = 1;
assert_checkequal(http_put(root+"/posts/1", data), data);

// HTTP PATCH
clear data;
clear exp_result;
data.title = "New title";

exp_result.userId = 1;
exp_result.id = 1;
exp_result.title = "New title";
exp_result.body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto";
assert_checkequal(http_patch(root+"/posts/1", data), exp_result);

// HTTP DELETE
assert_checkequal(http_delete(root+"/posts/1"), fromJSON("{}"));
