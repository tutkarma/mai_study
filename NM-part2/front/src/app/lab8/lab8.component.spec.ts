import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { Lab8Component } from './lab8.component';

describe('Lab8Component', () => {
  let component: Lab8Component;
  let fixture: ComponentFixture<Lab8Component>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ Lab8Component ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(Lab8Component);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
