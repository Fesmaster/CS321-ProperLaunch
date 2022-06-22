var assert = require('chai').assert;
var formatDate = require('../render').formatDate;

describe("render", function(){
	it("formats date", function (){
			assert.equal(formatDate("2022-06-12T10:00:00-05:00"),
				"2022-06-12");
	});
});
describe("render", function(){
	it("formats date", function (){
			assert.equal(formatDate("2002-10-29T10:00:00-05:00"),
				"2002-10-29");
	});
});
describe("render", function(){
	it("formats date", function (){
			assert.equal(formatDate("2016-09-30T10:00:00-05:00"),
				"2016-09-30");
	});
});
describe("render", function(){
	it("formats date", function (){
			assert.equal(formatDate("2004-08-22T10:00:00-05:00"),
				"2004-08-22");
	});
});
describe("render", function(){
	it("formats date", function (){
			assert.equal(formatDate("2006-07-02T10:00:00-05:00"),
				"2006-07-02");
	});
});
describe("render", function(){
	it("formats date", function (){
			assert.equal(formatDate("2018-03-01T10:00:00-05:00"),
				"2018-03-01");
	});
});
describe("render", function(){
	it("formats date", function (){
			assert.equal(formatDate("2020-03-04T10:00:00-05:00"),
				"2020-03-04");
	});
});
describe("render", function(){
	it("formats date", function (){
			assert.equal(formatDate("2009-08-11T10:00:00-05:00"),
				"2009-08-11");
	});
});
describe("render", function(){
	it("formats date", function (){
			assert.equal(formatDate("2005-07-04T10:00:00-05:00"),
				"2005-07-04");
	});
});
describe("render", function(){
	it("formats date", function (){
			assert.equal(formatDate("2015-03-10T09:30:15-10:00"),
				"2015-03-10");
	});
});
describe("render", function(){
	it("formats date", function (){
			assert.equal(formatDate("2015-35-10T09:30:15-10:00"),
				"NaN-NaN-NaN");
	});
});
