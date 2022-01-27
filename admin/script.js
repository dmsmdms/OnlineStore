function makePostRequest(data) {
    return new Promise(function (resolve, reject) {
        const xhr = new XMLHttpRequest();

        xhr.open('POST', '/api', true);
        xhr.setRequestHeader('Content-type', 'application/json');

        xhr.onload = function() {
            const content_type = xhr.getResponseHeader('Content-Type');
            let json = null;

            if (content_type === 'application/json') {
                json = JSON.parse(xhr.response);
            }

            if (xhr.status === 200 && json.ok !== false) {
                resolve(json);
            } else if (json !== null)  {
                console.error(`status: ${xhr.status}, response: ${json}`);
                reject(json);
            } else {
                console.error(`status: ${xhr.status}`);
                reject();
            }
        };

        xhr.onerror = function () {
            reject();
        };

        xhr.send(JSON.stringify(data));
    });
}

function getCategories() {
    return makePostRequest({ cmd: 'getCategories', data: {}});
}

function addCategory(category) {
    return makePostRequest({ cmd: 'addCategory', data: category });
}

function delCategory(id) {
    return makePostRequest({ cmd: 'delCategory', data: { id: parseInt(id) }});
}

function editCategory(category) {
    return makePostRequest({ cmd: 'editCategory', data: category });
}
