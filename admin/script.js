let categories = null;

const nameReg = /[а-яА-Я\s\w\d]{3,24}/;
const linkReg = /[\w]{3,24}/;

window.onload = function() {
	initCategoryAddDialog();
}

function initCategoryAddDialog() {
	const categoryAddDialog = document.getElementById('category-add-dialog');
	const categoryAddOpen = document.getElementById('category-add-open');
	const categoryAddClose = document.getElementById('category-add-close');
	const categoryAddButton = document.getElementById('category-add-button');
	const categoryAddForm = document.getElementById('category-add-form');

	categoryAddOpen.onclick = function() {
		categoryAddDialog.showModal();
	};

	categoryAddClose.onclick = function() {
		categoryAddDialog.close();
	};

	categoryAddButton.onclick = function() {
		const name = categoryAddForm.name.value;
		const link = categoryAddForm.link.value;
		let hasError = false;

		if (nameReg.test(name) === false) {
			categoryAddForm.name.classList.add('input-alert');
			hasError = true;
		} else {
			categoryAddForm.name.classList.remove('input-alert');
		}

		if (linkReg.test(link) === false) {
			categoryAddForm.link.classList.add('input-alert');
			hasError = true;
		} else {
			categoryAddForm.link.classList.remove('input-alert');
		}

		if (hasError === false) {
			addCategory({ name: name, link: link });
			categoryAddForm.name.value = null;
			categoryAddForm.link.value = null;
			categoryAddDialog.close();
		}
	};
}

async function updateCategoriesHTML() {
	const categoryUl = document.getElementById('categories-ul');
	categories = await getCategories();

	for (let i = 0; i < categories.length; ++i) {
	}
}

function makePostRequest(data) {
	return new Promise(function(resolve, reject) {
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
			} else if (json !== null) {
				console.error(`status: ${xhr.status}, response: ${json}`);
				reject(json);
			} else {
				console.error(`status: ${xhr.status}`);
				reject();
			}
		};

		xhr.onerror = function() {
			reject();
		};

		xhr.send(JSON.stringify(data));
	});
}

function getCategories() {
	const data = {};
	return makePostRequest({ cmd: 'getCategories', data: data });
}

function addCategory(category) {
	return makePostRequest({ cmd: 'addCategory', data: category });
}

function delCategory(id) {
	const data = { id: parseInt(id) };
	return makePostRequest({ cmd: 'delCategory', data: data });
}

function editCategory(category) {
	return makePostRequest({ cmd: 'editCategory', data: category });
}
